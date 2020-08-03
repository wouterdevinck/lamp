#pragma once

#include "driver/spi_master.h"
#include "esp_flash.h"

class SpiFlash {

  public:
    SpiFlash(spi_host_device_t host, int dmaChannel, int pinMosi, int pinMiso, int pinClk, int pinCs);
    virtual ~SpiFlash();
    bool init();
    bool erase(uint32_t address, uint32_t length);
    bool write(const void *buffer, uint32_t address, uint32_t length);

  private:
    spi_host_device_t _host;
    esp_flash_t* _flash;

};