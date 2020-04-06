// Helper library to work with esp-idf i2c
#pragma once

#include <driver/i2c.h>
#include "esp_log.h"
#include "constants.h"

#define ACK_CHECK_EN     0x1
#define ACK_CHECK_DIS    0x0
#define ACK_VAL          0x0
#define NACK_VAL         0x1

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t shared_i2c_init();
void      shared_i2c_wake();
void      shared_i2c_release();

esp_err_t shared_i2c_write(uint8_t addr, uint8_t reg);
esp_err_t shared_i2c_write_byte(uint8_t addr, uint8_t reg, uint8_t val);
esp_err_t shared_i2c_write_data(uint8_t addr, uint8_t *data, size_t len);

uint16_t  shared_i2c_read_word(uint8_t addr, uint8_t reg);
uint8_t   shared_i2c_read_byte(uint8_t addr, uint8_t reg);
esp_err_t shared_i2c_read_data(uint8_t addr, uint8_t *data, uint16_t *len);

#ifdef __cplusplus
}
#endif