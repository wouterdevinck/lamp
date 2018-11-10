using System.Threading;
using System.Windows.Controls;

namespace Lamp.Simulator {

    public partial class LedGroup : UserControl {

        public ushort _r;
        public ushort _g;
        public ushort _b;
        public ushort _w;

        public ushort R {
            get => _r;
            set {
                _r = value;
                UpdateLed();
            }
        }

        public ushort G {
            get => _g;
            set {
                _g = value;
                UpdateLed();
            }
        }

        public ushort B {
            get => _b;
            set {
                _b = value;
                UpdateLed();
            }
        }

        public ushort W {
            get => _w;
            set {
                _w = value;
                UpdateLed();
            }
        }

        public LedGroup() {
            InitializeComponent();
            ushort w = 0;
            new Thread(() => {
                while (w <= 4095) {
                    Dispatcher.Invoke(() => {
                        //R = w;
                        B = w;
                    });
                    Thread.Sleep(8); // ~120 fps
                    w += 10;
                }
            }).Start();
        }

        private void UpdateLed() {
            foreach (var led in new Led[] { led1, led2, led3, led4 }) {
                led.R = _r;
                led.G = _g;
                led.B = _b;
                led.W = _w;
            }
        }

    }

}
