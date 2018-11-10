using System;
using System.Collections.Generic;
using LampInterop;

namespace Lamp.Simulator {

    internal class LedArray : AbstractLedBoardChain {

        protected override void AddKeyframe(KeyFrameWrapper keyframe) {
            throw new NotImplementedException();
        }

        protected override ChainInfoWrapper GetChainInfo() {
            throw new NotImplementedException();
        }

        protected override void SetAllLeds(LedValueWrapper color) {
            throw new NotImplementedException();
        }

        protected override void SetAllLeds(List<LedValueWrapper> values) {
            throw new NotImplementedException();
        }

        protected override void SetBrightness(List<byte> values) {
            throw new NotImplementedException();
        }

        protected override void SetBrightness(byte brightness) {
            throw new NotImplementedException();
        }

    }

}
