// Inspired by: https://github.com/nkolban/esp32-snippets/tree/master/cpp_utils/SPI.h

#pragma once

#include <driver/spi_master.h>
#include <driver/gpio.h>

class SPI {

  public:
    SPI(spi_host_device_t host, int dmaChannel, int pinMosi, int pinMiso, int pinClk, int pinCs);
    virtual ~SPI();
    void transfer(uint8_t *data, size_t dataLen);
    uint8_t transferByte(uint8_t value);

  private:
    spi_host_device_t _host;
    spi_device_handle_t _handle;

};