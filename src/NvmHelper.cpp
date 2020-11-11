#include "NvmHelper.hpp"
#include <nvs.h>
#include <Arduino.h>

void NvmHelper::NvmLoad(const char *sNvsName, const char *sName, void *pSettings, unsigned nSettingsLen) {
  nvs_handle handle;
  noInterrupts();
  nvs_open(sNvsName, NVS_READONLY, &handle);
  size_t size = nSettingsLen;
  nvs_get_blob(handle, sName, pSettings, &size);
  nvs_close(handle);
  interrupts();
}

void NvmHelper::NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen) {
  nvs_handle handle;
  noInterrupts();
  nvs_open(sNvsName, NVS_READWRITE, &handle);
  nvs_set_blob(handle, sName, pSettings, nSettingsLen);
  nvs_commit(handle);
  nvs_close(handle);
  interrupts();
}

void NvmHelper::NvmErase(const char *sNvsName) {
  nvs_handle handle;
  noInterrupts();
  nvs_open(sNvsName, NVS_READWRITE, &handle);
  nvs_erase_all(handle);
  nvs_commit(handle);
  nvs_close(handle);
  interrupts();
}