#include "SpiFlash.h"
#include "esp_log.h"
#include "esp_flash.h"
#include "esp_flash_spi_init.h"

static char tag[] = "SpiFlash";

SpiFlash::SpiFlash(spi_host_device_t host, int dmaChannel, int pinMosi, int pinMiso, int pinClk, int pinCs) {

  _host = host;

  ESP_LOGD(tag, "mosi=%d, miso=%d, clk=%d, cs=%d", pinMosi, pinMiso, pinClk, pinCs);
  spi_bus_config_t bus_config;
  bus_config.sclk_io_num     = pinClk;
  bus_config.mosi_io_num     = pinMosi;
  bus_config.miso_io_num     = pinMiso;
  bus_config.quadwp_io_num   = -1;
  bus_config.quadhd_io_num   = -1;
  bus_config.max_transfer_sz = 0;
  bus_config.intr_flags      = 0;
  bus_config.flags           = (SPICOMMON_BUSFLAG_SCLK | SPICOMMON_BUSFLAG_MOSI | SPICOMMON_BUSFLAG_MISO);
  ESP_LOGI(tag, "Initializing bus - host=%d", host);
  esp_err_t err = ::spi_bus_initialize(host, &bus_config, dmaChannel);
  if (err != ESP_OK) {
  	ESP_LOGE(tag, "spi_bus_initialize(): rc=%d", err);
  	abort();
  }

  esp_flash_spi_device_config_t device_config;
  device_config.host_id = host;
  device_config.cs_id = 0;
  device_config.cs_io_num = pinCs;
  device_config.io_mode = SPI_FLASH_DIO; // SPI_FLASH_FASTRD;
  device_config.speed = ESP_FLASH_40MHZ;

  esp_flash_t* _flash;
  err = ::spi_bus_add_flash_device(&_flash, &device_config);
  if (err != ESP_OK) {
    ESP_LOGE(tag, "spi_bus_add_flash_device(): rc=%d", err);
    abort();
  }

  err = ::esp_flash_init(_flash);
  if (err != ESP_OK) {
    ESP_LOGE(tag, "esp_flash_init(): rc=%d", err);
    abort();
  }

  uint32_t id;
  ESP_ERROR_CHECK(::esp_flash_read_id(_flash, &id));
  ESP_LOGI(tag, "Initialized external Flash, size=%d KB, ID=0x%x", _flash->size / 1024, id);

}

SpiFlash::~SpiFlash() {
  //ESP_LOGI(tag, "Removing device");
  //ESP_ERROR_CHECK(::spi_bus_remove_device(_handle));
  ESP_LOGI(tag, "Freeing bus");
  ESP_ERROR_CHECK(::spi_bus_free(_host));
}
