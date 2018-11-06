#include "new.h"

#include "IrReceiver.h"
#include "RgbLed.h"
#include "LedBoardChain.h"
#include "Lamp.h"

#define DRIVERS 60

using namespace lamp;

extern "C" {
  int main(void);
}

int main(void) {
  auto ir = new IrReceiver();
  auto led = new RgbLed();
  auto leds = new LedBoardChain(DRIVERS);
  auto lamp = new Lamp(ir, led, leds);
  lamp->start(0);
  while(1){
    ir->loop();
  }
}