#include "stdafx.h"
#include "AbstractRgbLed.h"

using namespace LampInterop;

void NativeRgbLed::setLedColor(RgbLedColor color) {
	return m_owner->CallSetLedColor(color);
}