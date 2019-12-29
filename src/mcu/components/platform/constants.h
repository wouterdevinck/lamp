#pragma once

#define HTTP_PORT     (80)

#define CHUNK_SIZE    (15 * 1024)
#define MEMMON_PERIOD (60000)

#define PIN_RGB_BLUE  (33)
#define PIN_RGB_GREEN (32)
#define PIN_RGB_RED   (25)

#define PIN_LED_INT   (-1) // TODO

#define SPI1_HOST     HSPI_HOST
#define SPI1_DMA_CH   (1)
#define SPI1_MOSI_PIN (13)
#define SPI1_MISO_PIN (12)
#define SPI1_CLK_PIN  (14)
#define SPI1_CS_PIN   (15)

#define SPI2_HOST     VSPI_HOST
#define SPI2_DMA_CH   (2)
#define SPI2_MOSI_PIN (23)
#define SPI2_MISO_PIN (19)
#define SPI2_CLK_PIN  (18)
#define SPI2_CS_PIN   (5)

#define I2C_SDA_PIN   (21)
#define I2C_SCL_PIN   (22)

#define NVS_NS_IOT    "factory"
#define NVS_NS_STORE  "storage"

#define IOT_HSM_KID   (1)
#define IOT_HSM_IOKID (4)
#define IOT_DPS_URL   "global.azure-devices-provisioning.net"
#define IOT_DPS_IDS   "0ne00094B68"
#define IOT_NVS_KEY   "certificate"

#define NTP_SERVER    "pool.ntp.org"