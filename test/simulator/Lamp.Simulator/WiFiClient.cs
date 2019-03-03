using System;
using LampInterop;

namespace Lamp.Simulator {

    internal class WiFiClient : AbstractWiFiClient {

        private WiFiHandlerWrapper _handler;

        protected override void SetHandler(WiFiHandlerWrapper handler) {
            _handler = handler;
        }

        protected override void Connect(string ssid, string pwd) {
            _handler.OnConnected();
        }

        protected override void Reconnect() {
            _handler.OnConnected();
        }

        protected override void StartSmartConfig() {
            _handler.OnSmartConfig("boe", "bah");
        }

    }

}
