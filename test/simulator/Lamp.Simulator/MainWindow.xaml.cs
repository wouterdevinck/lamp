using LampInterop;

namespace Lamp.Simulator {

    public partial class MainWindow {

        private const int Port = 8282;

        public MainWindow() {
            InitializeComponent();

            var updater = new Updater();
            var httpserver = new HttpServer();
            var httpclient = new HttpClient();
            var led = new RgbLed(ellipse);
            var leds = new LedArray();

            var lamp = new LampWrapper(updater, httpserver, httpclient, led, leds); 
            
            lamp.Start(Port);

            browser.Navigate("http://localhost:" + Port);

        }

    }

}