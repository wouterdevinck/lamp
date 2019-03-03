using System;
using System.Windows;
using LampInterop;

namespace Lamp.Simulator {

    internal class Platform : AbstractPlatform {

        private Updater _updater;
        private Logger _logger;
        private IrReceiver _ir;
        private HttpServer _httpserver;
        private HttpClient _httpclient;
        private RgbLed _led;
        private LedArray _leds;
        private WiFiClient _wifi;
        private Nvs _nvs;
        private int _port;

        public IrReceiver IrReceiver => _ir;
        public Logger Logger => _logger;

        public Platform(int port, FrameworkElement led, LedDisplay ledDisplay) {
            _updater = new Updater();
            _logger = new Logger();
            _ir = new IrReceiver();
            _httpserver = new HttpServer();
            _httpclient = new HttpClient();
            _led = new RgbLed(led);
            _leds = new LedArray(ledDisplay);
            _wifi = new WiFiClient();
            _nvs = new Nvs();
            _port = port;
        }

        protected override AbstractIrReceiver GetIrReceiver() {
            return _ir;
        }

        protected override AbstractRgbLed GetRgbLed() {
            return _led;
        }

        protected override AbstractLedBoardChain GetLedBoardChain() {
            return _leds;
        }

        protected override AbstractWiFiClient GetWiFiClient() {
            return _wifi;
        }

        protected override AbstractUpdater GetUpdater() {
            return _updater;
        }

        protected override AbstractLogger GetLogger() {
            return _logger;
        }

        protected override AbstractHttpServer GetHttpServer() {
            return _httpserver;
        }

        protected override AbstractHttpClient GetHttpClient() {
            return _httpclient;
        }

        protected override AbstractNvs GetStorage() {
            return _nvs;
        }

        protected override int GetHttpServerPort() {
            return _port;
        }

    }

}
