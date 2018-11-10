#include "IrHandler.h"

using namespace lamp;
	
void IrHandler::handleIrCommand(uint16_t code) {
  switch(code) {
    case 0x0090: 
      _led->setLedColor({ 255, 0, 0 });
      _leds->changeColor(Red);
      break;
    case 0x0010:
      _led->setLedColor({ 0, 255, 0 });
      _leds->changeColor(Green);
      break;
    case 0x0050:
      _led->setLedColor({ 0, 0, 255 });
      _leds->changeColor(Blue);
      break;
    case 0x00D0:
      _led->setLedColor({ 255, 255, 255 });
      _leds->changeColor(White);
      break;
  }
}

/*

Remote control
==============

Row 1
-----
0x00A0 = Brightness Up
0x0020 = Brightness Down
0x0060 = Off
0x00E0 = On

Row 2
-----
0x0090 = Red
0x0010 = Green 1
0x0050 = Blue 4
0x00D0 = White

Row 3
-----
0x00B0 = Orange 1
0x0030 = Green 2
0x0070 = Blue 5
0x00F0 = Flash

Row 4
-----
0x00A8 = Orange 2
0x0028 = Blue 1
0x0068 = Purple 1
0x00E8 = Strobe

Row 5
-----
0x0098 = Orange 3
0x0018 = Blue 2
0x0058 = Purple 2
0x00D8 = Fade

Row 6
-----
0x0088 = Yellow
0x0008 = Blue 3
0x0048 = Purple 3
0x00C8 = Smooth

*/