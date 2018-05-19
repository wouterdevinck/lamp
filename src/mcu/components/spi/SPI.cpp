// Inspired by: https://github.com/nkolban/esp32-snippets/tree/master/cpp_utils/SPI.cpp

#include "SPI.h"
#include <esp_log.h>

static char tag[] = "SPI";

SPI::SPI(spi_host_device_t host, int dmaChannel, int pinMosi, int pinMiso, int pinClk, int pinCs) {
  _handle = nullptr;
  _host = host;
  ESP_LOGD(tag, "mosi=%d, miso=%d, clk=%d, cs=%d", pinMosi, pinMiso, pinClk, pinCs);
  spi_bus_config_t bus_config;
  bus_config.sclk_io_num     = pinClk;
  bus_config.mosi_io_num     = pinMosi;
  bus_config.miso_io_num     = pinMiso;
  bus_config.quadwp_io_num   = -1;
  bus_config.quadhd_io_num   = -1;
  bus_config.max_transfer_sz = 0;
  ESP_LOGI(tag, "Initializing bus - host=%d", host);
  esp_err_t errRc = ::spi_bus_initialize(host, &bus_config, dmaChannel);
  if (errRc != ESP_OK) {
  	ESP_LOGE(tag, "spi_bus_initialize(): rc=%d", errRc);
  	abort();
  }
  spi_device_interface_config_t dev_config;
  dev_config.address_bits     = 0;
  dev_config.command_bits     = 0;
  dev_config.dummy_bits       = 0;
  dev_config.mode             = 0;
  dev_config.duty_cycle_pos   = 0;
  dev_config.cs_ena_posttrans = 0;
  dev_config.cs_ena_pretrans  = 0;
  dev_config.clock_speed_hz   = 100000;
  dev_config.spics_io_num     = pinCs;
  dev_config.flags            = 0;
  dev_config.queue_size       = 1;
  dev_config.pre_cb           = NULL;
  dev_config.post_cb          = NULL;
  ESP_LOGI(tag, "Adding device bus");
  errRc = ::spi_bus_add_device(host, &dev_config, &_handle);
  if (errRc != ESP_OK) {
    ESP_LOGE(tag, "spi_bus_add_device(): rc=%d", errRc);
    abort();
  }
}

SPI::~SPI() {
  ESP_LOGI(tag, "Removing device");
  ESP_ERROR_CHECK(::spi_bus_remove_device(_handle));
  ESP_LOGI(tag, "Freeing bus");
  ESP_ERROR_CHECK(::spi_bus_free(_host));
}

void SPI::transfer(uint8_t* data, size_t dataLen) {
  assert(data != nullptr);
  assert(dataLen > 0);
  spi_transaction_t trans_desc;
  //trans_desc.address   = 0;
  //trans_desc.command   = 0;
  trans_desc.flags     = 0;
  trans_desc.length    = dataLen * 8;
  trans_desc.rxlength  = 0;
  trans_desc.tx_buffer = data;
  trans_desc.rx_buffer = data;
  esp_err_t rc = ::spi_device_transmit(_handle, &trans_desc);
  if (rc != ESP_OK) {
    ESP_LOGE(tag, "spi_device_transmit: %d", rc);
  }
}

uint8_t SPI::transferByte(uint8_t value) {
  transfer(&value, 1);
  return value;
} 