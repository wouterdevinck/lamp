using LampInterop;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;

namespace Lamp.Simulator {

    internal class RgbLed : AbstractRgbLed, INotifyPropertyChanged {

        private SolidColorBrush _brush;

        public SolidColorBrush Brush {
            get => _brush;
            private set {
                if (Equals(value, _brush)) return;
                _brush = value;
                NotifyPropertyChanged();
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public RgbLed(FrameworkElement wpf) {
            wpf.DataContext = this;
        }

        private void NotifyPropertyChanged([CallerMemberName] string propertyName = "") {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        protected override void SetLedColor(RgbLedColorWrapper color) {
            var br = new SolidColorBrush(Color.FromRgb(color.R, color.G, color.B));
            br.Freeze(); // Thread safety magic
            Dispatcher.CurrentDispatcher.Invoke(() => Brush = br);
        }

    }

}