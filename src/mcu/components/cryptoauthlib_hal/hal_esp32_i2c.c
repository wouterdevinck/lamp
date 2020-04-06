// Adapted from https://github.com/MicrochipTech/cryptoauthlib/blob/20190517/lib/hal/hal_esp32_i2c.c

#include <stdio.h>
#include <string.h>
#include "hal/atca_hal.h"
#include "esp_err.h"
#include "esp_log.h"
#include "constants.h"
#include "i2c.h"

const char* TAG = "CRYPTO_HAL_I2C";

void hal_i2c_change_baud(ATCAIface iface, uint32_t speed) {
  ESP_LOGV(TAG, "hal_i2c_change_baud");
}

ATCA_STATUS hal_i2c_init(void *hal, ATCAIfaceCfg *cfg) {
  ESP_LOGV(TAG, "hal_i2c_init");
  esp_err_t ret = shared_i2c_init();
  if (ret != ESP_OK) {
    return ATCA_COMM_FAIL;
  }
  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_post_init(ATCAIface iface) {
  ESP_LOGV(TAG, "hal_i2c_post_init");
  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_send(ATCAIface iface, uint8_t *txdata, int txlength) {
  ESP_LOGV(TAG, "hal_i2c_send");
  esp_err_t rc;
  txdata[0] = 0x03;
  txlength++;
  rc = shared_i2c_write_data(I2C_ADDR_ATCA, txdata, txlength);
  if (ESP_OK != rc) {
      return ATCA_COMM_FAIL;
  }
  return ATCA_SUCCESS;  
}

ATCA_STATUS hal_i2c_receive(ATCAIface iface, uint8_t *rxdata, uint16_t *rxlength) {
  ESP_LOGV(TAG, "hal_i2c_receive");
  esp_err_t rc = shared_i2c_read_data(I2C_ADDR_ATCA, rxdata, rxlength);
  if (ESP_OK != rc) {
    return ATCA_COMM_FAIL;
  } else {
    return ATCA_SUCCESS;
  }
}

ATCA_STATUS hal_i2c_release(void *hal_data) {
  ESP_LOGV(TAG, "hal_i2c_release");
  shared_i2c_release();
  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_wake(ATCAIface iface) {
  ESP_LOGV(TAG, "hal_i2c_wake");
  uint16_t rxlen;
  uint8_t data[4] = { 0 };
  const uint8_t expected[4] = { 0x04, 0x11, 0x33, 0x43 };
  shared_i2c_wake();
  atca_delay_ms(10); 
  rxlen = 4;
  hal_i2c_receive(iface, data, &rxlen);
  if (memcmp(data, expected, 4) == 0)     {
    ESP_LOGV(TAG, "I2C wake successful");
    return ATCA_SUCCESS;
  }
  ESP_LOGW(TAG, "I2C wake failed");
  return ATCA_COMM_FAIL;
}

ATCA_STATUS hal_i2c_idle(ATCAIface iface) {
  ESP_LOGV(TAG, "hal_i2c_idle");
  uint8_t idle_data = 0x02;
  shared_i2c_write(I2C_ADDR_ATCA, idle_data);
  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_sleep(ATCAIface iface) {
  ESP_LOGV(TAG, "hal_i2c_sleep");
  uint8_t sleep_data = 0x01;
  shared_i2c_write(I2C_ADDR_ATCA, sleep_data);
  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_discover_buses(int i2c_buses[], int max_buses) {
  ESP_LOGV(TAG, "hal_i2c_discover_buses");
 return ATCA_UNIMPLEMENTED;
}

ATCA_STATUS hal_i2c_discover_devices(int bus_num, ATCAIfaceCfg *cfg, int *found) {
  ESP_LOGV(TAG, "hal_i2c_discover_devices");
  return ATCA_UNIMPLEMENTED;
}
