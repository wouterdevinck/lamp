#include "stdafx.h"
#include "AbstractIotFramework.h"

using namespace LampInterop;

IIotClient* NativeIotFramework::connect(IIotHandler* handler) {
	return m_owner->CallConnect(handler);
}