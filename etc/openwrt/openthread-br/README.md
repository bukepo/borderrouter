OpenThread Border Router on OpenWRT
===================================

## Get Started

This is for local development.

1. Add OpenThread feed.

Suppose `OPENWRT_TOP_SRCDIR` is root of openwrt sources.

```bash
echo src-link openthread "$(pwd)/etc/openwrt" >> ${OPENWRT_TOP_SRCDIR}/feeds.conf
cd "${OPENWRT_TOP_SRCDIR}"
./scripts/feeds install -a
```
2. Enable OpenThread.

```bash
make menuconfig
```

Check *Network* > *OpenThread Border Router*.

3. Build OpenThread Border Router

```bash
make package/openthread/compile
```

or verbose make,

```bash
make -j1 V=sc package/openthread/compile
```

4. Install.

Copy the generated **ipk** file into OpenWRT, and install with **opkg**.

```bash
opkg install openthread-br-1.0*.ipk
```
