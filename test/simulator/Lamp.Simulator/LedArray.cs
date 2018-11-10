using System;
using System.Collections.Generic;
using LampInterop;

namespace Lamp.Simulator {

    internal class LedArray : AbstractLedBoardChain {

        private const Byte Boards = 30;
        private LedDisplay _disp;
        private ChainInfoWrapper _info;

        public LedArray(LedDisplay disp) {
            _disp = disp;
            _info = new ChainInfoWrapper(Boards);
        }

        protected override void AddKeyframe(KeyFrameWrapper keyframe) {
            throw new NotImplementedException();
        }

        protected override ChainInfoWrapper GetChainInfo() {
            return _info;
        }

        protected override void SetAllLeds(LedValueWrapper color) {
            var values = new List<LedValueWrapper>();
            for (var i = 0; i < _info.LedGroups; i++) {
                values.Add(color);
            }
            _disp.Values = values;
        }

        protected override void SetAllLeds(List<LedValueWrapper> values) {
            _disp.Values = values;
        }

        protected override void SetBrightness(List<byte> values) {
            throw new NotImplementedException();
        }

        protected override void SetBrightness(byte brightness) {
            throw new NotImplementedException();
        }

    }

}
