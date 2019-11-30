// Adapted from https://github.com/MicrochipTech/cryptoauthlib/blob/20190517/lib/hal/hal_esp32_i2c.c

#include <stdio.h>
#include <string.h>
#include <driver/i2c.h>
#include "hal/atca_hal.h"
#include "esp_err.h"
#include "esp_log.h"
#include "constants.h"

#define SDA_PIN          I2C_SDA_PIN
#define SCL_PIN          I2C_SCL_PIN
#define I2C_BUS          I2C_NUM_0
#define I2C_SPEED        100000
#define I2C_SLAVE_ADDR   0XC0

#define ACK_CHECK_EN     0x1
#define ACK_CHECK_DIS    0x0
#define ACK_VAL          0x0
#define NACK_VAL         0x1

const char* TAG = "CRYPTO_HAL_I2C";

i2c_config_t conf;

void hal_i2c_change_baud(ATCAIface iface, uint32_t speed) {
  ESP_LOGV(TAG, "hal_i2c_change_baud");
}

ATCA_STATUS hal_i2c_init(void *hal, ATCAIfaceCfg *cfg) {
  ESP_LOGV(TAG, "hal_i2c_init");
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = SDA_PIN;
  conf.scl_io_num = SCL_PIN;
  conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
  conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
  conf.master.clk_speed = I2C_SPEED;
  (void)i2c_param_config(I2C_BUS, &conf);
  (void)i2c_driver_install(I2C_BUS, I2C_MODE_MASTER, 0, 0, 0);
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
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  (void)i2c_master_start(cmd);
  (void)i2c_master_write_byte(cmd, I2C_SLAVE_ADDR | I2C_MASTER_WRITE, ACK_CHECK_EN);
  (void)i2c_master_write(cmd, txdata, txlength, ACK_CHECK_EN);
  (void)i2c_master_stop(cmd);
  rc = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  (void)i2c_cmd_link_delete(cmd);
  if (ESP_OK != rc) {
      return ATCA_COMM_FAIL;
  } else {
      return ATCA_SUCCESS;
  }  
}

ATCA_STATUS hal_i2c_receive(ATCAIface iface, uint8_t *rxdata, uint16_t *rxlength) {
  ESP_LOGV(TAG, "hal_i2c_receive");
  esp_err_t rc;
  i2c_cmd_handle_t cmd;
  int high = 0;
  int low = 0;
  cmd = i2c_cmd_link_create();
  (void)i2c_master_start(cmd);
  (void)i2c_master_write_byte(cmd, I2C_SLAVE_ADDR | I2C_MASTER_READ, ACK_CHECK_EN);
  (void)i2c_master_read_byte(cmd, rxdata, ACK_VAL);
  rc = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  (void)i2c_cmd_link_delete(cmd);
  if (ESP_OK != rc) {
    return ATCA_COMM_FAIL;
  }
  *rxlength = rxdata[0];
  if (*rxlength > 1) {
    cmd = i2c_cmd_link_create();
    if (*rxlength > 2) {
      (void)i2c_master_read(cmd, &rxdata[1], (*rxlength) - 2, ACK_VAL);
    }
    (void)i2c_master_read_byte(cmd, rxdata + (*rxlength) - 1, NACK_VAL);
    (void)i2c_master_stop(cmd);
    rc = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
    (void)i2c_cmd_link_delete(cmd);
  } else {
    cmd = i2c_cmd_link_create();
    (void)i2c_master_stop(cmd);
    rc = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
    (void)i2c_cmd_link_delete(cmd);
  }
  if (ESP_OK != rc) {
    return ATCA_COMM_FAIL;
  } else {
    return ATCA_SUCCESS;
  }
}

ATCA_STATUS hal_i2c_release(void *hal_data) {
  ESP_LOGV(TAG, "hal_i2c_release");
  i2c_driver_delete(I2C_BUS);
  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_wake(ATCAIface iface) {
  ESP_LOGV(TAG, "hal_i2c_wake");
  uint16_t rxlen;
  uint8_t data[4] = { 0 };
  const uint8_t expected[4] = { 0x04, 0x11, 0x33, 0x43 };
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  (void)i2c_master_start(cmd);
  (void)i2c_master_write_byte(cmd, I2C_MASTER_WRITE, ACK_CHECK_DIS);
  (void)i2c_master_stop(cmd);
  (void)i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  (void)i2c_cmd_link_delete(cmd);
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
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  (void)i2c_master_start(cmd);
  (void)i2c_master_write_byte(cmd, I2C_SLAVE_ADDR | I2C_MASTER_WRITE, ACK_CHECK_EN);
  (void)i2c_master_write(cmd, &idle_data, 1, ACK_CHECK_DIS);
  (void)i2c_master_stop(cmd);
  (void)i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  (void)i2c_cmd_link_delete(cmd);
  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_sleep(ATCAIface iface) {
  ESP_LOGV(TAG, "hal_i2c_sleep");
  uint8_t sleep_data = 0x01;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  (void)i2c_master_start(cmd);
  (void)i2c_master_write_byte(cmd, I2C_SLAVE_ADDR | I2C_MASTER_WRITE, ACK_CHECK_EN);
  (void)i2c_master_write(cmd, &sleep_data, 1, ACK_CHECK_DIS);
  (void)i2c_master_stop(cmd);
  (void)i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  (void)i2c_cmd_link_delete(cmd);
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
