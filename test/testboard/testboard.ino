#include "DM633.h"

#define DMNUMBER 2
#define LATCHPIN 7

LedDriver LedBoard(DMNUMBER, LATCHPIN);

void setup() {
  Serial.begin(9200);
  LedBoard.init();
  LedBoard.setGlobalBrightness(100);
}

void loop() {
  allRed();
  allGreen();
  allBlue();
  allWhite();
  cycleAll();
}

void allRed()   { all(0); }
void allGreen() { all(1); }
void allBlue()  { all(2); }
void allWhite() { all(3); }

void all(int color) {
  LedBoard.clearAll();
  for (int i = DMNUMBER - 1; i >= 0; --i) {
    for (int k = color; k < 16; k += 4) {
      LedBoard.setChannelValue((i * 16) + k, 4095);
    }
  }
  LedBoard.sendAll();
  delay(1000);
}

void cycleAll() {
  for (int i = DMNUMBER - 1; i >= 0; --i) {
    for (int k = 0; k < 16; ++k) {
      LedBoard.clearAll();
      LedBoard.setChannelValue((i * 16) + k, 4095);
      LedBoard.sendAll();
      delay(300);
    }
  }
}
