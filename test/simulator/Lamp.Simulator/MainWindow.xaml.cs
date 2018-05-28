using LampInterop;

namespace Lamp.Simulator {

    public partial class MainWindow { 

        public MainWindow() {
            InitializeComponent();

            var updater = new Updater();
            var httpserver = new HttpServer();
            var httpclient = new HttpClient();
            var led = new RgbLed(ellipse);
            var leds = new LedArray();

            var lamp = new LampWrapper(updater, httpserver, httpclient, led, leds); 
            
            lamp.Start(8282);

        }

    }

}