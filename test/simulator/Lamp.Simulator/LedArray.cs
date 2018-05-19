using System;
using System.Collections.Generic;
using LampInterop;

namespace Lamp.Simulator {

    internal class LedArray : AbstractLedBoardChain {

        protected override void AddKeyframe(long milliseconds, List<LedValueWrapper> keyframe) {
            throw new NotImplementedException();
        }

    }

}
