using LampInterop;

namespace Lamp.Simulator {

    public class IotFramework : AbstractIotFramework {

        private IotHandlerWrapper _handler;

        protected override AbstractIotClient Connect(IotHandlerWrapper handler) {
            _handler = handler;
            return new IotClient();
        }

    }

}