#ifndef __BLEDEVICE_EX_H__
#define __BLEDEVICE_EX_H__

// 内部関数などにアクセスするためにprivateを無効化する
#define private public
#include "BLEDevice.h"
#undef private

std::map<uint16_t, BLERemoteCharacteristic*>* retrieveCharacteristicsEx( BLERemoteService* pRemoteService ) {
  ESP_LOGD(LOG_TAG, ">> retrieveCharacteristics() for service: %s", getUUID().toString().c_str());

  pRemoteService->removeCharacteristics(); // Forget any previous characteristics.

  uint16_t offset = 0;
  esp_gattc_char_elem_t result;
  while (true) {
    uint16_t count = 1;  // this value is used as in parameter that allows to search max 10 chars with the same uuid
    esp_gatt_status_t status = ::esp_ble_gattc_get_all_char(
      pRemoteService->getClient()->getGattcIf(),
      pRemoteService->getClient()->getConnId(),
      pRemoteService->m_startHandle,
      pRemoteService->m_endHandle,
      &result,
      &count,
      offset
    );

    if (status == ESP_GATT_INVALID_OFFSET || status == ESP_GATT_NOT_FOUND) {   // We have reached the end of the entries.
      break;
    }

    if (status != ESP_GATT_OK) {   // If we got an error, end.
      ESP_LOGE(LOG_TAG, "esp_ble_gattc_get_all_char: %s", BLEUtils::gattStatusToString(status).c_str());
      break;
    }

    if (count == 0) {   // If we failed to get any new records, end.
      break;
    }

    ESP_LOGD(LOG_TAG, "Found a characteristic: Handle: %d, UUID: %s", result.char_handle, BLEUUID(result.uuid).toString().c_str());

    // We now have a new characteristic ... let us add that to our set of known characteristics
    BLERemoteCharacteristic *pNewRemoteCharacteristic = new BLERemoteCharacteristic(
      result.char_handle,
      BLEUUID(result.uuid),
      result.properties,
      pRemoteService
    );

    pRemoteService->m_characteristicMap.insert(std::pair<std::string, BLERemoteCharacteristic*>(pNewRemoteCharacteristic->getUUID().toString(), pNewRemoteCharacteristic));
    pRemoteService->m_characteristicMapByHandle.insert(std::pair<uint16_t, BLERemoteCharacteristic*>(result.char_handle, pNewRemoteCharacteristic));
    offset++;   // Increment our count of number of descriptors found.
  } // Loop forever (until we break inside the loop).

  pRemoteService->m_haveCharacteristics = true; // Remember that we have received the characteristics.
  ESP_LOGD(LOG_TAG, "<< retrieveCharacteristics()");

  return &pRemoteService->m_characteristicMapByHandle;
} // retrieveCharacteristicsEx

#endif
