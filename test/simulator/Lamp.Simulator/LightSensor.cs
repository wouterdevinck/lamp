using System;
using LampInterop;

namespace Lamp.Simulator {

    internal class LightSensor : AbstractLightSensor {

        protected override UInt32 GetIlluminanceInLux() {
            return 50;
        }

    }

}