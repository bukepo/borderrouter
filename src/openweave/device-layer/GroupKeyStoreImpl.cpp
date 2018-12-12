/*
 *
 *    Copyright (c) 2018 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *          Provides an implementation of the Weave GroupKeyStore interface
 *          for the ESP32 platform.
 */

#include "GroupKeyStoreImpl.h"
#include <Weave/DeviceLayer/internal/WeaveDeviceLayerInternal.h>

using namespace ::nl;
using namespace ::nl::Weave;
using namespace ::nl::Weave::Profiles::Security::AppKeys;

namespace nl {
namespace Weave {
namespace DeviceLayer {
namespace Internal {

WEAVE_ERROR GroupKeyStoreImpl::RetrieveGroupKey(uint32_t keyId, WeaveGroupKey &key)
{
    WEAVE_ERROR      err;
    size_t           keyLen;
    char             keyName[kMaxConfigKeyNameLength + 1];
    ESP32Config::Key configKey{kConfigNamespace_WeaveConfig, keyName};

    err = FormKeyName(keyId, keyName, sizeof(keyName));
    SuccessOrExit(err);

    err = ReadConfigValueBin(configKey, key.Key, sizeof(key.Key), keyLen);
    if (err == WEAVE_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        err = WEAVE_ERROR_KEY_NOT_FOUND;
    }
    SuccessOrExit(err);

    if (keyId != WeaveKeyId::kFabricSecret)
    {
        memcpy(&key.StartTime, key.Key + kWeaveAppGroupKeySize, sizeof(uint32_t));
        keyLen -= sizeof(uint32_t);
    }

    key.KeyId  = keyId;
    key.KeyLen = keyLen;

exit:
    return err;
}

WEAVE_ERROR GroupKeyStoreImpl::StoreGroupKey(const WeaveGroupKey &key)
{
    WEAVE_ERROR err;
    char        keyName[kMaxConfigKeyNameLength + 1];
    uint8_t     keyData[WeaveGroupKey::MaxKeySize];
    bool        needClose    = false;
    bool        indexUpdated = false;

    fprintf(stderr, "%s 1\r\n", __func__);
    err = FormKeyName(key.KeyId, keyName, sizeof(keyName));
    SuccessOrExit(err);

    fprintf(stderr, "%s 2\r\n", __func__);
    err = AddKeyToIndex(key.KeyId, indexUpdated);
    SuccessOrExit(err);

    fprintf(stderr, "%s 3\r\n", __func__);
    memcpy(keyData, key.Key, WeaveGroupKey::MaxKeySize);
    if (key.KeyId != WeaveKeyId::kFabricSecret)
    {
        memcpy(keyData + kWeaveAppGroupKeySize, (const void *)&key.StartTime, sizeof(uint32_t));
    }

    fprintf(stderr, "%s 4\r\n", __func__);
#if WEAVE_PROGRESS_LOGGING
    if (WeaveKeyId::IsAppEpochKey(key.KeyId))
    {
        WeaveLogProgress(DeviceLayer,
                         "GroupKeyStore: storing epoch key %s/%s (key len %" PRId8 ", start time %" PRIu32 ")",
                         kConfigNamespace_WeaveConfig, keyName, key.KeyLen, key.StartTime);
    }
    else if (WeaveKeyId::IsAppGroupMasterKey(key.KeyId))
    {
        WeaveLogProgress(DeviceLayer,
                         "GroupKeyStore: storing app master key %s/%s (key len %" PRId8 ", global id 0x%" PRIX32 ")",
                         kConfigNamespace_WeaveConfig, keyName, key.KeyLen, key.GlobalId);
    }
    else
    {
        const char *keyType = (WeaveKeyId::IsAppRootKey(key.KeyId)) ? "root" : "general";
        WeaveLogProgress(DeviceLayer, "GroupKeyStore: storing %s key %s/%s (key len %" PRId8 ")", keyType,
                         kConfigNamespace_WeaveConfig, keyName, key.KeyLen);
    }
#endif // WEAVE_PROGRESS_LOGGING
    fprintf(stderr, "%s 5\r\n", __func__);

exit:
    if (err != WEAVE_NO_ERROR && indexUpdated)
    {
        mNumKeys--;
    }
    ClearSecretData(keyData, sizeof(keyData));
    return err;
}

WEAVE_ERROR GroupKeyStoreImpl::DeleteGroupKey(uint32_t keyId)
{
    return DeleteKeyOrKeys(keyId, WeaveKeyId::kType_None);
}

WEAVE_ERROR GroupKeyStoreImpl::DeleteGroupKeysOfAType(uint32_t keyType)
{
    return DeleteKeyOrKeys(WeaveKeyId::kNone, keyType);
}

WEAVE_ERROR GroupKeyStoreImpl::EnumerateGroupKeys(uint32_t  keyType,
                                                  uint32_t *keyIds,
                                                  uint8_t   keyIdsArraySize,
                                                  uint8_t & keyCount)
{
    keyCount = 0;

    for (uint8_t i = 0; i < mNumKeys && keyCount < keyIdsArraySize; i++)
    {
        if (keyType == WeaveKeyId::kType_None || WeaveKeyId::GetType(mKeyIndex[i]) == keyType)
        {
            keyIds[keyCount++] = mKeyIndex[i];
        }
    }

    return WEAVE_NO_ERROR;
}

WEAVE_ERROR GroupKeyStoreImpl::Clear(void)
{
    return DeleteKeyOrKeys(WeaveKeyId::kNone, WeaveKeyId::kType_None);
}

WEAVE_ERROR GroupKeyStoreImpl::RetrieveLastUsedEpochKeyId(void)
{
    WEAVE_ERROR err;

    err = ReadConfigValue(kConfigKey_LastUsedEpochKeyId, LastUsedEpochKeyId);
    if (err == WEAVE_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        LastUsedEpochKeyId = WeaveKeyId::kNone;
        err                = WEAVE_NO_ERROR;
    }
    return err;
}

WEAVE_ERROR GroupKeyStoreImpl::StoreLastUsedEpochKeyId(void)
{
    return WriteConfigValue(kConfigKey_LastUsedEpochKeyId, LastUsedEpochKeyId);
}

WEAVE_ERROR GroupKeyStoreImpl::Init()
{
    WEAVE_ERROR err;
    size_t      indexSizeBytes;

    err = ReadConfigValueBin(kConfigKey_GroupKeyIndex, (uint8_t *)mKeyIndex, sizeof(mKeyIndex), indexSizeBytes);
    if (err == WEAVE_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        err            = WEAVE_NO_ERROR;
        indexSizeBytes = 0;
    }
    SuccessOrExit(err);

    mNumKeys = indexSizeBytes / sizeof(uint32_t);

exit:
    return err;
}

WEAVE_ERROR GroupKeyStoreImpl::AddKeyToIndex(uint32_t keyId, bool &indexUpdated)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;

    indexUpdated = false;

    for (uint8_t i = 0; i < mNumKeys; i++)
    {
        if (mKeyIndex[i] == keyId)
        {
            ExitNow(err = WEAVE_NO_ERROR);
        }
    }

    VerifyOrExit(mNumKeys < kMaxGroupKeys, err = WEAVE_ERROR_TOO_MANY_KEYS);

    mKeyIndex[mNumKeys++] = keyId;
    indexUpdated          = true;

exit:
    return err;
}

WEAVE_ERROR GroupKeyStoreImpl::DeleteKeyOrKeys(uint32_t targetKeyId, uint32_t targetKeyType)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR GroupKeyStoreImpl::FormKeyName(uint32_t keyId, char *buf, size_t bufSize)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;

    VerifyOrExit(bufSize >= kMaxConfigKeyNameLength, err = WEAVE_ERROR_BUFFER_TOO_SMALL);

    if (keyId == WeaveKeyId::kFabricSecret)
    {
        strcpy(buf, kConfigKey_FabricSecret.Name);
    }
    else
    {
        snprintf(buf, bufSize, "%s%08" PRIX32, kGroupKeyNamePrefix, keyId);
    }

exit:
    return err;
}

} // namespace Internal
} // namespace DeviceLayer
} // namespace Weave
} // namespace nl
