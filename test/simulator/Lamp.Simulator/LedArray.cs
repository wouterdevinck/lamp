using System;
using System.Collections.Generic;
using LampInterop;
using System.Threading;
using System.Windows;

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

        private delegate void NoArgDelegate();

        public static void Refresh(DependencyObject obj) {

            obj.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.ApplicationIdle,

                (NoArgDelegate)delegate { });

        }

        protected override void SetAllLeds(LedValueWrapper color) {
            var values = new List<LedValueWrapper>();
            for (var i = 0; i < _info.LedGroups; i++) {
                values.Add(color);
            }
            //(new Thread(() => {})).Start();
            _disp.Values = values;
            Refresh(_disp);
            // Thread.Sleep(20000);
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
