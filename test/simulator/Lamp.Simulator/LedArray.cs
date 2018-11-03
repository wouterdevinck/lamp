using System;
using LampInterop;

namespace Lamp.Simulator {

    internal class LedArray : AbstractLedBoardChain {

        protected override void AddKeyframe(KeyFrameWrapper keyframe) {
            throw new NotImplementedException();
        }

        protected override void SetAllLeds(LedValueWrapper color) {
            throw new NotImplementedException();
        }

    }

}
