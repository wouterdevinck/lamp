// Adapted from https://github.com/adafruit/Adafruit_TSL2591_Library/blob/master/Adafruit_TSL2591.h

#include "TSL2591.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "constants.h"
#include "i2c.h"

using namespace lamp;

TSL2591::TSL2591() {
  _initialized = false;
  _integration = TSL2591_INTEGRATIONTIME_100MS;
  _gain        = TSL2591_GAIN_MED;
}

bool TSL2591::begin() {
  if (::shared_i2c_init() != ESP_OK) {
    return false;
  }
  uint8_t id = ::shared_i2c_read_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_ID);
  if (id != 0x50) {
    return false;
  }
  _initialized = true;
  setTiming(_integration);
  setGain(_gain);
  disable();
  return true;
}

void TSL2591::enable() {
  if (!_initialized) {
    if (!begin()) {
      return;
    }
  }
  ::shared_i2c_write_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE,
	  TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN | TSL2591_ENABLE_NPIEN);
}

void TSL2591::disable() {
  if (!_initialized) {
    if (!begin()) {
      return;
    }
  }
  ::shared_i2c_write_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE, TSL2591_ENABLE_POWEROFF);
}

void TSL2591::setGain(tsl2591Gain_t gain) {
  if (!_initialized) {
    if (!begin()) {
      return;
    }
  }
  enable();
  _gain = gain;
  ::shared_i2c_write_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL, _integration | _gain);
  disable();
}

tsl2591Gain_t TSL2591::getGain() {
  return _gain;
}

void TSL2591::setTiming(tsl2591IntegrationTime_t integration) {
  if (!_initialized) {
    if (!begin()) {
      return;
    }
  }
  enable();
  _integration = integration;
  ::shared_i2c_write_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL, _integration | _gain);
  disable();
}

tsl2591IntegrationTime_t TSL2591::getTiming() {
  return _integration;
}

float TSL2591::calculateLux(uint16_t ch0, uint16_t ch1) {
  float    atime, again;
  float    cpl, lux;
  if ((ch0 == 0xFFFF) | (ch1 == 0xFFFF)) {
    return -1;
  }
  switch (_integration) {
    case TSL2591_INTEGRATIONTIME_100MS :
      atime = 100.0F;
      break;
    case TSL2591_INTEGRATIONTIME_200MS :
      atime = 200.0F;
      break;
    case TSL2591_INTEGRATIONTIME_300MS :
      atime = 300.0F;
      break;
    case TSL2591_INTEGRATIONTIME_400MS :
      atime = 400.0F;
      break;
    case TSL2591_INTEGRATIONTIME_500MS :
      atime = 500.0F;
      break;
    case TSL2591_INTEGRATIONTIME_600MS :
      atime = 600.0F;
      break;
    default: 
      atime = 100.0F;
      break;
  }
  switch (_gain) {
    case TSL2591_GAIN_LOW :
      again = 1.0F;
      break;
    case TSL2591_GAIN_MED :
      again = 25.0F;
      break;
    case TSL2591_GAIN_HIGH :
      again = 428.0F;
      break;
    case TSL2591_GAIN_MAX :
      again = 9876.0F;
      break;
    default:
      again = 1.0F;
      break;
  }
  cpl = (atime * again) / TSL2591_LUX_DF;
  lux = ( ((float)ch0 - (float)ch1 )) * (1.0F - ((float)ch1/(float)ch0) ) / cpl;
  return lux;
}

uint32_t TSL2591::getFullLuminosity() {
  if (!_initialized) {
    if (!begin()) {
      return 0;
    }
  }
  enable();
  for (uint8_t d=0; d<=_integration; d++) {
    ::vTaskDelay(120 / portTICK_PERIOD_MS);
  }
  uint32_t x;
  uint16_t y;
  y = ::shared_i2c_read_word(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW);
  x = ::shared_i2c_read_word(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW);
  x <<= 16;
  x |= y;
  disable();
  return x;
}

uint16_t TSL2591::getLuminosity(uint8_t channel) {
  uint32_t x = getFullLuminosity();
  if (channel == TSL2591_FULLSPECTRUM) {
    return (x & 0xFFFF);
  } else if (channel == TSL2591_INFRARED) {
    return (x >> 16);
  } else if (channel == TSL2591_VISIBLE) {
    return ( (x & 0xFFFF) - (x >> 16));
  }
  return 0;
}

void TSL2591::registerInterrupt(uint16_t lowerThreshold, uint16_t upperThreshold, tsl2591Persist_t persist = TSL2591_PERSIST_ANY) {
  if (!_initialized) {
    if (!begin()) {
      return;
    }
  }
  enable();
  ::shared_i2c_write_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_PERSIST_FILTER,  persist);
  ::shared_i2c_write_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AILTL, lowerThreshold);
  ::shared_i2c_write_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AILTH, lowerThreshold >> 8);
  ::shared_i2c_write_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AIHTL, upperThreshold);
  ::shared_i2c_write_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AIHTH, upperThreshold >> 8);
  disable();
}

void TSL2591::clearInterrupt() {
  if (!_initialized) {
    if (!begin()) {
      return;
    }
  }
  enable();
  ::shared_i2c_write(I2C_ADDR_TSL, TSL2591_CLEAR_INT);
  disable();
}

uint8_t TSL2591::getStatus() {
  if (!_initialized) {
    if (!begin()) {
      return 0;
    }
  }
  enable();
  uint8_t x;
  x = ::shared_i2c_read_byte(I2C_ADDR_TSL, TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_STATUS);
  disable();
  return x;
}