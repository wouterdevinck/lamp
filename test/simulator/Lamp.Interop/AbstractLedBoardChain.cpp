#include "stdafx.h"
#include "AbstractLedBoardChain.h"

using namespace LampInterop;
using namespace lamp;
using namespace std;

void NativeLedBoardChain::addKeyframe(milliseconds duration, vector<LedValue> keyframe) {
	return m_owner->CallAddKeyframe(duration, keyframe);
}