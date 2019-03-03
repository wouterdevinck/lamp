using System.Collections.Generic;
using LampInterop;

namespace Lamp.Simulator {

    internal class Nvs: AbstractNvs {

        private readonly Dictionary<string, string> _store = new Dictionary<string, string>();

        protected override string GetValue(string key) {
            return _store.ContainsKey(key) ? _store[key] : "";
        }

        protected override void SetValue(string key, string value) {
            _store[key] = value;
        }

    }

}
