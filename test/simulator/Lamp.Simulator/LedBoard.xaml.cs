using LampInterop;
using System;
using System.Collections.Generic;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;

namespace Lamp.Simulator {

    public partial class LedBoard : UserControl {

        private const double Gamma = 2.8;
        private Func<ushort, byte> DownSample = x =>
            (byte)((Math.Pow((double)x / 4095, 1 / Gamma) * 255) + 0.5);

        public List<LedValueWrapper> Values {
            set {
                if (value.Count != 8) throw new Exception();
                var i = 0;
                foreach (var group in new Rectangle[] {
                    Group0, Group1, Group2, Group3,
                    Group4, Group5, Group6, Group7
                }) {
                    var val = value[i];
                    if (val.W > 0) {
                        var w = DownSample(val.W);
                        Dispatcher.Invoke(() => {
                            group.Fill = new SolidColorBrush(Color.FromRgb(w, w, 0));
                        });
                    } else {
                        var r = DownSample(val.R);
                        var g = DownSample(val.G);
                        var b = DownSample(val.B);
                        Dispatcher.Invoke(() => {
                            group.Fill = new SolidColorBrush(Color.FromRgb(r, g, b));
                        });
                    }
                    i++;
                }
            }
        }

        public LedBoard() {
            InitializeComponent();
        }

    }

}
