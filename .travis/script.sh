#!/bin/sh
#
#  Copyright (c) 2017, The OpenThread Authors.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. Neither the name of the copyright holder nor the
#     names of its contributors may be used to endorse or promote products
#     derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#

set -e
set -x

TOOLS_HOME=$HOME/.cache/tools

die() {
	echo " *** ERROR: " $*
	exit 1
}

case $BUILD_TARGET in
android-check)
    (cd .. && "${TRAVIS_BUILD_DIR}/.travis/android_check")
    ;;

pretty-check)
    export PATH=$TOOLS_HOME/usr/bin:$PATH || die
    ./bootstrap || die
    ./configure && make pretty-check || die
    ;;

posix-check)
    export CPPFLAGS="$CFLAGS -I$TOOLS_HOME/usr/include"
    export LDFLAGS="$LDFLAGS -L$TOOLS_HOME/usr/lib"
    ./bootstrap || die
    if [ "$WITH_MDNS" = 'mDNSResponder' ]; then
        sudo service avahi-daemon stop
        sudo mdnsd
        ./configure && make distcheck DISTCHECK_CONFIGURE_FLAGS='--with-mdns=mDNSResponder'
    else
        ./configure && make distcheck
    fi
    ;;

scan-build)
    SCAN_TMPDIR=./scan-tmp
    ./bootstrap || die
    scan-build ./configure --disable-docs || die
    scan-build -o $SCAN_TMPDIR -analyze-headers -v make || die
    grep '^<!-- BUGFILE '$TRAVIS_BUILD_DIR $SCAN_TMPDIR/*/report-*.html | tee bugfiles
    [ `< bugfiles grep -v 'third_party' | wc -l` = '0' ] || die
    ;;

script-check)
    docker run -d --name systemd --security-opt seccomp=unconfined --tmpfs /run --tmpfs /run/lock -v /sys/fs/cgroup:/sys/fs/cgroup:ro -t solita/ubuntu-systemd
    docker cp . systemd:/root/borderrouter
    docker exec -it systemd sh -c 'apt-get update; apt-get install -y git; cd /root/borderrouter; .travis/script_check'
    ;;

raspbian-gcc)
    IMAGE_FILE=$IMAGE_NAME.img
    STAGE_DIR=/tmp/raspbian
    IMAGE_DIR=/media/rpi

    [ -d $STAGE_DIR ] || mkdir -p $STAGE_DIR
    cp -v $TOOLS_HOME/images/$IMAGE_FILE $STAGE_DIR/raspbian.img || die
    git archive -o $STAGE_DIR/repo.tar HEAD || die

    cat > $STAGE_DIR/check.sh <<EOF
        export LC_ALL=C
        export DEBIAN_FRONTEND=noninteractive
        export RELEASE=1

        echo "127.0.0.1    \$(hostname)" >> /etc/hosts
        mount -t proc none /proc
        mount -t sysfs none /sys
        chown -R pi:pi /home/pi/repo
        cd /home/pi/repo
        apt-get update
        apt-get install -y git
        su -m -c 'git config --global pack.threads 1' pi
        su -m -c 'script/bootstrap' pi
        su -m -c './configure --prefix=/usr' pi
        su -m -c 'make' pi
EOF

    (cd docker-rpi-emu/scripts &&
        sudo mkdir -p $IMAGE_DIR &&                                    \
        sudo ./mount.sh $STAGE_DIR/raspbian.img $IMAGE_DIR &&          \
        sudo mount --bind /dev/pts $IMAGE_DIR/dev/pts &&               \
        sudo mkdir -p $IMAGE_DIR/home/pi/repo &&                       \
        sudo tar xvf $STAGE_DIR/repo.tar -C $IMAGE_DIR/home/pi/repo && \
        sudo git clone --depth 1 https://github.com/openthread/wpantund.git $IMAGE_DIR/home/pi/repo/._build/wpantund && \
        sudo cp -v $STAGE_DIR/check.sh $IMAGE_DIR/home/pi/check.sh &&  \
        sudo ./qemu-setup.sh $IMAGE_DIR &&                             \
        sudo chroot $IMAGE_DIR /bin/bash /home/pi/check.sh &&          \
        sudo ./qemu-cleanup.sh $IMAGE_DIR &&                           \
        sudo ./unmount.sh $IMAGE_DIR) || die
    ;;

*)
    die
    ;;
esac
