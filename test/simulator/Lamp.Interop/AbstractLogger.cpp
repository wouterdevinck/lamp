#include "stdafx.h"
#include "AbstractLogger.h"

using namespace LampInterop;

void NativeLogger::logError(string tag, string line) {
	return m_owner->CallLogError(tag, line);
}

void NativeLogger::logWarning(string tag, string line) {
	return m_owner->CallLogWarning(tag, line);
}

void NativeLogger::logInfo(string tag, string line) {
	return m_owner->CallLogInfo(tag, line);
}

void NativeLogger::logDebug(string tag, string line) {
	return m_owner->CallLogDebug(tag, line);
}

void NativeLogger::logVerbose(string tag, string line) {
	return m_owner->CallLogVerbose(tag, line);
}