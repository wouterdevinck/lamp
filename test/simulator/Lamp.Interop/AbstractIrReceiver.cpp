#include "stdafx.h"
#include "AbstractIrReceiver.h"

using namespace LampInterop;

void NativeIrReceiver::start(IIrHandler* handler) {
	return m_owner->CallStart(handler);
}