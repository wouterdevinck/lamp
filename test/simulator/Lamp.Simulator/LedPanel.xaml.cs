using LampInterop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Controls;

namespace Lamp.Simulator {

    public partial class LedPanel : UserControl {

        public List<LedValueWrapper> Values {
            set {
                if (value.Count != 15 * 8) throw new Exception();
                var i = 0;
                foreach (var board in new LedBoard[] {
                    Board0, Board1, Board2, Board3, Board4,
                    Board5, Board6, Board7, Board8, Board9,
                    Board10, Board11, Board12, Board13, Board14
                }) {
                    board.Values = value.Skip(i * 8).Take(8).ToList();
                    i++;
                }
            }
        }

        public LedPanel() {
            InitializeComponent();
        }

    }

}
