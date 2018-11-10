using System;
using System.Windows.Controls;
using System.Windows.Media;

namespace Lamp.Simulator {

    public partial class Led : UserControl {

        private const double Gamma = 2.8;

        public ushort _r;
        public ushort _g;
        public ushort _b;
        public ushort _w;

        public ushort R {
            get => _r;
            set {
                if (value > 4095) throw new Exception();
                _r = value;
                UpdateLed();
            }
        }

        public ushort G {
            get => _g;
            set {
                if (value > 4095) throw new Exception();
                _g = value;
                UpdateLed();
            }
        }

        public ushort B {
            get => _b;
            set {
                if (value > 4095) throw new Exception();
                _b = value;
                UpdateLed();
            }
        }

        public ushort W {
            get => _w;
            set {
                if (value > 4095) throw new Exception();
                _w = value;
                UpdateLed();
            }
        }

        public Led() {
            InitializeComponent();
        }

        private Func<ushort, byte> DownSample = x => 
            (byte)((Math.Pow((double)x / 4095, 1 / Gamma) * 127) + 127);

        private void UpdateLed() {
            if (_w == 0) {
                white.Fill = Brushes.Transparent;
            } else {
                var w = DownSample(_w);
                var color = Color.FromRgb(w, w, 0);
                white.Fill = new SolidColorBrush(color);
                shadow.Color = Color.FromRgb(255, 255, 0);
            }
            if (_r == 0 && _g == 0 && _b == 0) {
                rgb.Fill = Brushes.Transparent;
            } else {
                var r = DownSample(_r);
                var g = DownSample(_g);
                var b = DownSample(_b);
                var color = Color.FromRgb(r, g, b);
                rgb.Fill = new SolidColorBrush(color);
                shadow.Color = color;
            }
            shadow.BlurRadius = (_r + _g + _b + _w) / 410;
        }

    }

}
