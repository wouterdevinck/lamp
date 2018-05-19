#pragma once

#include "IRgbLed.h"
#include "driver/ledc.h"

#define LEDC_TIMER          LEDC_TIMER_0
#define LEDC_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_CH1_CHANNEL    LEDC_CHANNEL_1
#define LEDC_CH2_CHANNEL    LEDC_CHANNEL_2
#define LEDC_CH_NUM         (3)

typedef struct {
  ledc_channel_t channel;
  int io;
  ledc_mode_t mode;
  ledc_timer_t timer_idx;
} ledc_info_t;

namespace lamp {

  class RgbLed : public IRgbLed {

    public:
      explicit RgbLed(int pinRed, int pinGreen, int pinBlue);

      void setLedColor(RgbLedColor color) override;

  };

}