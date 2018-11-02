#include "new.h"
//#include <util/delay.h>

#include "RgbLed.h"
#include "Lamp.h"

using namespace lamp;

extern "C" {
  int main(void);
}

int main(void) {
  auto led = new RgbLed();
  auto lamp = new Lamp(led);
  lamp->start(0);
  while(1){
    /*for(uint8_t i = 0; i < 255; i++) {
      led->setLedColor({ i, 0, 0 });
      _delay_ms(10);
    }
    for(uint8_t i = 0; i < 255; i++) {
      led->setLedColor({ 0, i, 0 });
      _delay_ms(10);
    };
    for(uint8_t i = 0; i < 255; i++) {
      led->setLedColor({ 0, 0, i });
      _delay_ms(10);
    }
    for(uint8_t i = 0; i < 255; i++) {
      led->setLedColor({ i, i, i });
      _delay_ms(10);
    }*/
  }
}