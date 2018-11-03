#include "stdafx.h"
#include "AbstractLedBoardChain.h"

using namespace LampInterop;

void NativeLedBoardChain::addKeyframe(KeyFrame keyframe) {
	return m_owner->CallAddKeyframe(keyframe);
}

void NativeLedBoardChain::setAllLeds(LedValue color) {
	return m_owner->CallSetAllLeds(color);
}