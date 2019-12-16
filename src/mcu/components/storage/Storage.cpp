#include "Storage.h"
#include "constants.h"

using namespace lamp;
using namespace std;

Storage::Storage() {
  esp_err_t ret = ::nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(::nvs_flash_erase());
    ret = ::nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  ESP_ERROR_CHECK(::nvs_open(NVS_NS_STORE, NVS_READWRITE, &_handle));
}

string Storage::getValue(string key) {
  size_t required_size;
  auto err = ::nvs_get_str(_handle, key.c_str(), NULL, &required_size);
  if (err != ESP_OK) return string();
  char* val = (char*)malloc(required_size);
  err = ::nvs_get_str(_handle, key.c_str(), val, &required_size);
  if (err != ESP_OK) return string();
  return string(val);
}

void Storage::setValue(string key, string value) {
  auto err = ::nvs_set_str(_handle, key.c_str(), value.c_str());
  if (err != ESP_OK) return;
  ::nvs_commit(_handle);
}