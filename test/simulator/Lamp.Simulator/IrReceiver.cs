using System;
using LampInterop;

namespace Lamp.Simulator {

    public class IrReceiver : AbstractIrReceiver {

        private IrHandlerWrapper _handler;

        protected override void Start(IrHandlerWrapper handler) {
            _handler = handler;
        }

        public void SendCode(Int16 code) {
            _handler.HandleIrCommand(code);
        }

    }

}
