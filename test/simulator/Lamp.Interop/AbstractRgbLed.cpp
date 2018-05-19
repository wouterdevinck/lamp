#include "stdafx.h"
#include "AbstractRgbLed.h"

using namespace LampInterop;
using namespace lamp;
using namespace std;

void NativeRgbLed::setLedColor(RgbLedColor color) {
	return m_owner->CallSetLedColor(color);
}