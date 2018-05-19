#include <cstdint>
#include "LedBoardChain.h"
#include "esp_log.h"

#if CONFIG_LOG_DEFAULT_LEVEL >= 4
  #include <iomanip>
  #include <sstream>
#endif

static char tag[] = "LedBoardChain";

using namespace lamp;

LedBoardChain::LedBoardChain(SPI* spi, int pinInt) {
  _spi = spi;
  _ready = true;

  // TODO
  // addISRHandler
  // setInterruptType PIO_INTR_NEGEDGE
  // interruptEnable
}

void LedBoardChain::addKeyframe(KeyFrame keyframe) {
  _frames.push(keyframe);
  if(_ready && !_frames.empty()) transferNextKeyframe();
}

void LedBoardChain::transferNextKeyframe() {
  auto f = _frames.front();
  _frames.pop();
  int cnt = f.frame.size();
  int dur = (f.duration.count() * FRAMERATE) / 1000;
  int cmdLen = (cnt * 6) + 2;
  int bufferLen = cmdLen + 2;
  ESP_LOGD(tag, "Building message for animation of %d leds in %d frames. Total length: %d bytes.", cnt, dur, bufferLen);
  uint8_t buffer[bufferLen];
  buffer[0] = ((uint8_t)Command::SendFrame << 3) + ((cmdLen & 2047) >> 8); 
  buffer[1] = cmdLen;
  buffer[2] = ((uint8_t)f.type << 2) + ((dur & 1023) >> 8);
  buffer[3] = dur;
  for (int i = 0; i < cnt; ++i) {
    auto led = f.frame[i];
    buffer[i * 6 + 4] = led.r >> 4;
    buffer[i * 6 + 5] = ((led.r & 15) << 4) + (led.g >> 8);
    buffer[i * 6 + 6] = led.g;
    buffer[i * 6 + 7] = led.b >> 4;
    buffer[i * 6 + 8] = ((led.b & 15) << 4) + (led.w >> 8);
    buffer[i * 6 + 9] = led.w;
  }
  #if CONFIG_LOG_DEFAULT_LEVEL >= 4
    ostringstream dbg;
    dbg << endl;
    for (int i = 0; i < bufferLen; ++i) {
      dbg << hex << setw(2) << setfill('0') << 
        static_cast<unsigned int>(buffer[i]) << ' ';
      if ((i + 1) % 8 == 0) {
        dbg << endl;
      }
    }
    ESP_LOGD(tag, "%s", dbg.str().c_str());
  #endif
  _spi->transfer(buffer, bufferLen);
}