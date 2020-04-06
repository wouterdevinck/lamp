#include "stdafx.h"
#include "AbstractLightSensor.h"

using namespace LampInterop;

UInt32 NativeLightSensor::getIlluminanceInLux() {
	return m_owner->CallGetIlluminanceInLux();
}