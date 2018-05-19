#include "RgbLed.h"

using namespace lamp;

RgbLed::RgbLed(int pinRed, int pinGreen, int pinBlue) {
  ledc_info_t ledc_ch[LEDC_CH_NUM] = {
    {
      .channel   = LEDC_CH0_CHANNEL,
      .io        = pinBlue,
      .mode      = LEDC_MODE,
      .timer_idx = LEDC_TIMER
    }, {
      .channel   = LEDC_CH1_CHANNEL,
      .io        = pinGreen,
      .mode      = LEDC_MODE,
      .timer_idx = LEDC_TIMER
    }, {
      .channel   = LEDC_CH2_CHANNEL,
      .io        = pinRed,
      .mode      = LEDC_MODE,
      .timer_idx = LEDC_TIMER
    }
  };
  ledc_timer_config_t ledc_timer;
  ledc_timer.speed_mode = LEDC_MODE;
  ledc_timer.duty_resolution = LEDC_TIMER_13_BIT;
  ledc_timer.timer_num = LEDC_TIMER;
  ledc_timer.freq_hz = 5000;
  ::ledc_timer_config(&ledc_timer);
  for (int ch = 0; ch < LEDC_CH_NUM; ch++) {
    ledc_channel_config_t ledc_channel = {
      .gpio_num = ledc_ch[ch].io,
      .speed_mode = ledc_ch[ch].mode,
      .channel = ledc_ch[ch].channel,
      .intr_type = LEDC_INTR_FADE_END,
      .timer_sel = ledc_ch[ch].timer_idx,
      .duty = 0
    };
    ::ledc_channel_config(&ledc_channel);
  }
  ::ledc_fade_func_install(0);
}

void RgbLed::setLedColor(RgbLedColor color) {
  ::ledc_set_fade_with_time(LEDC_MODE, LEDC_CH0_CHANNEL, color.b * 32, color.b == 0 ? 1000 : 3000);
  ::ledc_fade_start(LEDC_MODE, LEDC_CH0_CHANNEL, LEDC_FADE_NO_WAIT);
  ::ledc_set_fade_with_time(LEDC_MODE, LEDC_CH1_CHANNEL, color.g * 32, color.g == 0 ? 1000 : 3000);
  ::ledc_fade_start(LEDC_MODE, LEDC_CH1_CHANNEL, LEDC_FADE_NO_WAIT);
  ::ledc_set_fade_with_time(LEDC_MODE, LEDC_CH2_CHANNEL, color.r * 32, color.r == 0 ? 1000 : 3000);
  ::ledc_fade_start(LEDC_MODE, LEDC_CH2_CHANNEL, LEDC_FADE_NO_WAIT);
}