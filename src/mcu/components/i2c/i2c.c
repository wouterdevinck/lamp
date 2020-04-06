#include "i2c.h"

static const char *TAG = "I2C";

bool inited = false;

esp_err_t shared_i2c_init() {
  ESP_LOGV(TAG, "shared_i2c_init");
  if(!inited) {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_SDA_PIN;
    conf.scl_io_num = I2C_SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
    conf.master.clk_speed = I2C_SPEED;
    esp_err_t ret = i2c_param_config(I2C_BUS, &conf);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "i2c_param_config %d", ret);
    } else {
      ret = i2c_driver_install(I2C_BUS, I2C_MODE_MASTER, 0, 0, 0);
      if (ret != ESP_OK) {
        ESP_LOGE(TAG, "i2c_driver_install %d", ret);
      } else {
        inited = true;
        return ESP_OK;
      }
    }
  } else {
    return ESP_OK;
  }
  return ESP_FAIL;
}

void shared_i2c_wake() {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, I2C_MASTER_WRITE, ACK_CHECK_DIS);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  i2c_cmd_link_delete(cmd);
}

void shared_i2c_release() {
  i2c_driver_delete(I2C_BUS);
}

esp_err_t shared_i2c_write(uint8_t addr, uint8_t reg) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, addr<<1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
  uint8_t txdata[1] = { reg };
  i2c_master_write(cmd, txdata, 1, ACK_CHECK_EN); // or write_byte
  i2c_master_stop(cmd);
  esp_err_t ret = i2c_master_cmd_begin(I2C_BUS, cmd, 10); 
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "i2c_master_cmd_begin %d", ret);
  }
  i2c_cmd_link_delete(cmd);
  return ret;
}

esp_err_t shared_i2c_write_byte(uint8_t addr, uint8_t reg, uint8_t val) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, addr<<1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
  uint8_t txdata[2] = { reg, val };
  i2c_master_write(cmd, txdata, 2, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  esp_err_t ret = i2c_master_cmd_begin(I2C_BUS, cmd, 10); 
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "i2c_master_cmd_begin %d", ret);
  }
  i2c_cmd_link_delete(cmd);
  return ret;
}

esp_err_t shared_i2c_write_data(uint8_t addr, uint8_t *data, size_t len) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, addr<<1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
  i2c_master_write(cmd, data, len, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  esp_err_t ret  = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "i2c_master_cmd_begin %d", ret);
  }
  i2c_cmd_link_delete(cmd);
  return ret;
}

uint16_t shared_i2c_read_word(uint8_t addr, uint8_t reg) {
  uint8_t data[2] = { 0 };
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, addr<<1, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  esp_err_t ret = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  i2c_cmd_link_delete(cmd);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "i2c_master_cmd_begin %d", ret);
    return 0;
  }
  cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, addr<<1 | I2C_MASTER_READ, ACK_CHECK_EN);
  i2c_master_read(cmd, data, 2,  I2C_MASTER_LAST_NACK);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  i2c_cmd_link_delete(cmd);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "i2c_master_cmd_begin %d", ret);
    return 0;
  }
  uint16_t val = ((uint16_t)data[1] << 8) | data[0];
  ESP_LOGV(TAG, "Read data: 0x%02x", val);
  return val;
}

uint8_t shared_i2c_read_byte(uint8_t addr, uint8_t reg) {
  uint8_t data;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, addr<<1, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  esp_err_t ret = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  i2c_cmd_link_delete(cmd);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "i2c_master_cmd_begin %d", ret);
    return 0;
  }
  cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, addr<<1 | I2C_MASTER_READ, ACK_CHECK_EN);
  i2c_master_read(cmd, &data, 1,  I2C_MASTER_LAST_NACK);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  i2c_cmd_link_delete(cmd);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "i2c_master_cmd_begin %d", ret);
    return 0;
  }
  ESP_LOGV(TAG, "Read data: 0x%02x", data);
  return data;
}

esp_err_t shared_i2c_read_data(uint8_t addr, uint8_t *data, uint16_t *len) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, addr<<1 | I2C_MASTER_READ, ACK_CHECK_EN);
  i2c_master_read_byte(cmd, data, ACK_VAL);
  esp_err_t ret = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
  i2c_cmd_link_delete(cmd);
  if (ret != ESP_OK) {
    // ESP_LOGE(TAG, "i2c_master_cmd_begin %d", ret);
    return ESP_FAIL;
  }
  *len = data[0];
  if (*len > 1) {
    cmd = i2c_cmd_link_create();
    if (*len > 2) {
      i2c_master_read(cmd, &data[1], (*len) - 2, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data + (*len) - 1, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
    i2c_cmd_link_delete(cmd);
  } else {
    cmd = i2c_cmd_link_create();
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_BUS, cmd, 10);
    i2c_cmd_link_delete(cmd);
  }
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "i2c_master_cmd_begin %d", ret);
    return ESP_FAIL;
  } 
  return ESP_OK;
}