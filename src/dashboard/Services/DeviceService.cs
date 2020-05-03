using Microsoft.Azure.Devices;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace LampControl.Services {

    public class DeviceService {

        private ServiceClient _service;
        private RegistryManager _registry;

        public DeviceService(string hub, string user, string key) {
            var connectionString = $"HostName={hub};SharedAccessKeyName={user};SharedAccessKey={key}";
            _service = ServiceClient.CreateFromConnectionString(connectionString);
            _registry = RegistryManager.CreateFromConnectionString(connectionString);
        }

        public async IAsyncEnumerable<string> GetDevicesAsync() {
            var query = _registry.CreateQuery("SELECT * FROM devices WHERE status = 'enabled'", 100);
            while (query.HasMoreResults) {
                var page = await query.GetNextAsTwinAsync();
                foreach (var twin in page) {
                    yield return twin.DeviceId;
                }
            }
        }

        public async Task SetStatusLedColorAsync(string deviceId, string color) {
            var methodInvocation = new CloudToDeviceMethod("statusLed") { ResponseTimeout = TimeSpan.FromSeconds(30) };
            methodInvocation.SetPayloadJson("\"" + color + "\"");
            await _service.InvokeDeviceMethodAsync(deviceId, methodInvocation);
        }

    }

}
