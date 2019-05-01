#include "IrHandler.h"

using namespace lamp;
	
void IrHandler::handleIrCommand(uint16_t code) {
  switch(code) {
    case (uint16_t)IrCommand::BrightnessUp:
      _leds->increaseBrightness();
      break;
    case (uint16_t)IrCommand::BrightnessDown:
      _leds->decreaseBrightness();
      break;
    case (uint16_t)IrCommand::Red: 
      _led->setLedColor({ 255, 0, 0 });
      _leds->changeColor(LampColor::Red);
      break;
    case (uint16_t)IrCommand::Green1:
      _led->setLedColor({ 0, 255, 0 });
      _leds->changeColor(LampColor::Green);
      break;
    case (uint16_t)IrCommand::Blue4:
      _led->setLedColor({ 0, 0, 255 });
      _leds->changeColor(LampColor::Blue);
      break;
    case (uint16_t)IrCommand::White:
      _led->setLedColor({ 255, 255, 255 });
      _leds->changeColor(LampColor::White);
      break;
  }
}
