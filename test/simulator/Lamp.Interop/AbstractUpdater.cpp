#include "stdafx.h"
#include "AbstractUpdater.h"

using namespace LampInterop;

string NativeUpdater::getRunningVersion() {
	return m_owner->CallGetRunningVersion();
}

string NativeUpdater::getRunningFpgaHash() {
	return m_owner->CallGetRunningFpgaHash();
}

string NativeUpdater::getInstalledFpgaHash() {
	return m_owner->CallGetInstalledFpgaHash();
}

bool NativeUpdater::beginUpgrade() {
	return m_owner->CallBeginUpgrade();
}

bool NativeUpdater::writeChunk(vector<uint8_t> chunk) {
	return m_owner->CallWriteChunk(chunk);
}

bool NativeUpdater::completeUpgrade() {
	return m_owner->CallCompleteUpgrade();
}

bool NativeUpdater::flashFpga() {
	return m_owner->CallFlashFpga();
}