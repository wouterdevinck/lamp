using LampInterop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Controls;

namespace Lamp.Simulator {

    public partial class LedDisplay : UserControl {

        public List<LedValueWrapper> Values {
            set {
                if (value.Count != 30 * 8) throw new Exception();
                Top.Values = value.Take(15 * 8).ToList();
                Bottom.Values = value.Skip(15 * 8).ToList();
            }
        }

        public LedDisplay() {
            InitializeComponent();
        }

    }

}
