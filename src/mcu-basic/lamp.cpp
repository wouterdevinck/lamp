#include "new.h"

#include "IrReceiver.h"
#include "RgbLed.h"
#include "Lamp.h"

using namespace lamp;

extern "C" {
  int main(void);
}

int main(void) {
  auto ir = new IrReceiver();
  auto led = new RgbLed();
  auto lamp = new Lamp(ir, led);
  lamp->start(0);
  while(1){
    ir->loop();
  }
}