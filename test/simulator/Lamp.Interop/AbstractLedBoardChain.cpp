#include "stdafx.h"
#include "AbstractLedBoardChain.h"

using namespace LampInterop;

void NativeLedBoardChain::addKeyframe(KeyFrame keyframe) {
	m_owner->CallAddKeyframe(keyframe);
}

void NativeLedBoardChain::setAllLeds(LedValue color) {
	m_owner->CallSetAllLeds(color);
}

void NativeLedBoardChain::setAllLeds(LedValue values[]) {
	m_owner->CallSetAllLeds(values);
}

void NativeLedBoardChain::setBrightness(uint8_t brightness) {
	m_owner->CallSetBrightness(brightness);
}

void NativeLedBoardChain::setBrightness(uint8_t values[]) {
	m_owner->CallSetBrightness(values);
}

ChainInfo* NativeLedBoardChain::getChainInfo() {
	return m_owner->CallGetChainInfo();
}