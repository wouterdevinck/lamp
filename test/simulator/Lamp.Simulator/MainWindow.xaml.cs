using LampInterop;

namespace Lamp.Simulator {

    public partial class MainWindow { 

        public MainWindow() {
            InitializeComponent();

            var updater = new Updater();
            var http = new HttpServer();
            var led = new RgbLed(ellipse);
            var leds = new LedArray();

            var lamp = new LampWrapper(updater, http, led, leds); 
            
            lamp.Start(8282);

        }

    }

}