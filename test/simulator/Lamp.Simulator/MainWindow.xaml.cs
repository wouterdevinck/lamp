using LampInterop;

namespace Lamp.Simulator {

    public partial class MainWindow { 

        public MainWindow() {
            InitializeComponent();
            
            var http = new HttpServer();
            var led = new RgbLed(ellipse);
            var leds = new LedArray();

            var lamp = new LampWrapper(http, led, leds); 
            
            lamp.Start(8282);

        }

    }

}