using System.Windows.Controls;

namespace Lamp.Simulator {

    public partial class RemoteControl : UserControl {

        public IrReceiver Ir { get; set; }

        public RemoteControl() {
            InitializeComponent();
            btnRed.Click += (s, e) => { Send(0x0090); };
            btnGreen.Click += (s, e) => { Send(0x0010); };
            btnBlue.Click += (s, e) => { Send(0x0050); };
            btnWhite.Click += (s, e) => { Send(0x00D0); };
            btnFlash.Click += (s, e) => { Send(0x00F0); };
        }

        private void Send(short code) {
            if (Ir != null) {
                Ir.SendCode(code);
            }
        }

    }

}
