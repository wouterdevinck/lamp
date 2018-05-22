#include "stdafx.h"
#include "AbstractLedBoardChain.h"

using namespace LampInterop;
using namespace lamp;
using namespace std;

void NativeLedBoardChain::addKeyframe(KeyFrame keyframe) {
	return m_owner->CallAddKeyframe(keyframe);
}