#pragma once

#include "driver/spi_master.h"
//#include "esp_flash.h"

// TODO inherit Spi, add SpiMaster

class SpiFlash {

  public:
    SpiFlash(spi_host_device_t host, int dmaChannel, int pinMosi, int pinMiso, int pinClk, int pinCs);
    virtual ~SpiFlash();

  private:
    spi_host_device_t _host;
    //esp_flash_t* _flash;

};