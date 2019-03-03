#include "stdafx.h"
#include "AbstractNvs.h"

using namespace LampInterop;

string NativeNvs::getValue(string key) {
	return m_owner->CallGetValue(key);
}

void NativeNvs::setValue(string key, string value) {
	m_owner->CallSetValue(key, value);
}