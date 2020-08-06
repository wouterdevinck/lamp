#include "Updater.h"
#include "version.h"
#include "constants.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "freertos/task.h"

// #include <iomanip>
// #include <sstream>

static char tag[] = "Updater";

using namespace lamp;
using namespace std;

Updater::Updater(SpiFlash* flash, Storage* storage, uint16_t chunkSize) {
  _flash = flash;
  _storage = storage;
  _chunkSize = chunkSize;
  ESP_LOGD(tag, "FPGA version: %s", FPGA_HASH);
  ESP_LOGD(tag, "Lamp version: %s", LAMP_VERSION);
  /* ostringstream dbg;
  dbg << endl;
  for (int i = 0; i < fpga_image_end - fpga_image_start; ++i) {
    dbg << hex << setw(2) << setfill('0') << 
      static_cast<unsigned int>(fpga_image_start[i]) << ' ';
    if ((i + 1) % 8 == 0) {
      dbg << endl;
    }
  }
  ESP_LOGD(tag, "%s", dbg.str().c_str())*/
}

string Updater::getRunningVersion() {
  return LAMP_VERSION;
}

string Updater::getRunningFpgaHash() {
  return _storage->getValue(FPGA_NVS_KEY);
}

string Updater::getInstalledFpgaHash() {
  return FPGA_HASH;
}

bool Updater::beginUpgrade() {
  ESP_LOGD(tag, "Begin upgrade called");
  _otaTarget = ::esp_ota_get_next_update_partition(NULL);
  esp_err_t err = ::esp_ota_begin(_otaTarget, OTA_SIZE_UNKNOWN, &_otaHandle);
  if (err != ESP_OK) {
    ESP_LOGE(tag, "Begin upgrade failed!");
    return false;
  }
  ESP_LOGI(tag, "Begin upgrade succeeded");
  return true;
}

bool Updater::writeChunk(vector<uint8_t> chunk) {
  ESP_LOGD(tag, "Write chunk called");
  /* ostringstream dbg;
  for (int i = 0; i < chunk.size(); ++i) {
    dbg << hex << setw(2) << setfill('0') << 
      static_cast<unsigned int>(chunk.data()[i]);
  }
  ESP_LOGD(tag, "Chunk: %s", dbg.str().c_str()); */
  esp_err_t err = ::esp_ota_write(_otaHandle, (const void *)chunk.data(), chunk.size());
  if (err != ESP_OK) {
    ESP_LOGE(tag, "Writing chunk failed!");
    return false;
  }
  ESP_LOGD(tag, "Chunk written - %d bytes", chunk.size());
  return true;
}

bool Updater::completeUpgrade() {
  ESP_LOGD(tag, "Complete upgrade called");
  esp_err_t err = ::esp_ota_end(_otaHandle);
  if (err != ESP_OK) {
    ESP_LOGE(tag, "Complete upgrade failed (esp_ota_end)!");
    return false;
  }
  err = ::esp_ota_set_boot_partition(_otaTarget);
  if (err != ESP_OK) {
    ESP_LOGE(tag, "Complete upgrade failed (esp_ota_set_boot_partition)!");
    return false;
  }
  ESP_LOGI(tag, "Restarting...");
  ::esp_restart();
  return true;
}

bool Updater::flashFpga() {
  ESP_LOGD(tag, "Flashing FPGA...");
  ::gpio_reset_pin((gpio_num_t)PIN_FPGA_RESET);
  ::gpio_set_direction((gpio_num_t)PIN_FPGA_RESET, GPIO_MODE_OUTPUT);
  ::gpio_set_level((gpio_num_t)PIN_FPGA_RESET, 1);
  ::vTaskDelay(30 / portTICK_PERIOD_MS);
  ::gpio_set_level((gpio_num_t)PIN_FPGA_RESET, 0);
  bool res = _flash->init();
  if (!res) {
    return false;
  }
  uint len = fpga_image_end - fpga_image_start;
  res = _flash->erase(0, roundUp(len * 2, 4096));
  if (!res) {
    return false;
  }
  res = _flash->write(fpga_image_start, 0, len);
  if (!res) {
    return false;
  }
  ::gpio_set_level((gpio_num_t)PIN_FPGA_RESET, 1);
  ESP_LOGD(tag, "Completed flashing FPGA");
  _storage->setValue(FPGA_NVS_KEY, FPGA_HASH);
  return true;
}

uint16_t Updater::getPreferredChunkSize() {
  return _chunkSize;
}

int Updater::roundUp(int numToRound, int multiple) {
  if (multiple == 0) return numToRound;
  int remainder = numToRound % multiple;
  if (remainder == 0) return numToRound;
  return numToRound + multiple - remainder;
}