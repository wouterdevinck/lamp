#include "LightSensor.h"
#include "esp_log.h"

using namespace lamp;

static char tag[] = "LightSensor";

LightSensor::LightSensor() {
  _tsl = TSL2591();
  if(!_tsl.begin()) {
    ESP_LOGE(tag, "TSL2591 initialization failed");
  } else {
    _tsl.setGain(TSL2591_GAIN_LOW);
    _tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);
  }
}

uint32_t LightSensor::getIlluminanceInLux() {
  uint16_t vi = _tsl.getLuminosity(TSL2591_VISIBLE);
  uint16_t ir = _tsl.getLuminosity(TSL2591_INFRARED);
  return static_cast<int>(_tsl.calculateLux(vi, ir));
}
