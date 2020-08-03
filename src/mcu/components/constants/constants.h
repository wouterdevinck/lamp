#pragma once

// HTTP server port
#define HTTP_PORT      (80)

// Chunk size for downloading large files
#define CHUNK_SIZE     (15 * 1024)

// Frequency of free memory report
#define MEMMON_PERIOD  (60000)

// RGB status LED
#define PIN_RGB_BLUE   (33)
#define PIN_RGB_GREEN  (32)
#define PIN_RGB_RED    (25)

// Interrupt from FPGA to request data
#define PIN_LED_INT    (39)

// FPGA reset and configuration status pins
#define PIN_FPGA_RESET (15)
#define PIN_FPGA_CDONE (23)

// Connected to FPGA GPIO pins
#define SPI1_ESP_HOST  HSPI_HOST
#define SPI1_DMA_CH    (1)
#define SPI1_MOSI_PIN  (14)
#define SPI1_MISO_PIN  (27)
#define SPI1_CLK_PIN   (26)
#define SPI1_CS_PIN    (13)

// Connected to FPGA configuration SPI flash
#define SPI2_ESP_HOST  VSPI_HOST
#define SPI2_DMA_CH    (2)
#define SPI2_MOSI_PIN  (18)
#define SPI2_MISO_PIN  (16)
#define SPI2_CLK_PIN   (19)
#define SPI2_CS_PIN    (4)

// IC2 bus - light sensor (TSL) and crypto chip (ATCA)
#define I2C_BUS        I2C_NUM_0
#define I2C_SDA_PIN    (21)
#define I2C_SCL_PIN    (22)
#define I2C_SPEED      100000
#define I2C_ADDR_ATCA  0XC0 >> 1
#define I2C_ADDR_TSL   0X29

// Non volatile storage namespaces
#define NVS_NS_IOT     "factory"
#define NVS_NS_STORE   "storage"

// Azure IoT settings
#define IOT_HSM_KID    (1)
#define IOT_HSM_IOKID  (4)
#define IOT_DPS_URL    "global.azure-devices-provisioning.net"
#define IOT_DPS_IDS    "0ne00094B68"
#define IOT_NVS_KEY    "certificate"

// NTP server
#define NTP_SERVER     "pool.ntp.org"

// Upgrade
#define FPGA_NVS_KEY   "fpgahash"