using System.Threading.Tasks;
using LampControl.Services;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace LampControl.Controllers {

    public class HomeController : Controller {

        private readonly ILogger<HomeController> _logger;
        private readonly DeviceService _device;

        public HomeController(ILogger<HomeController> logger, DeviceService device) {
            _logger = logger;
            _device = device;
        }

        public IActionResult Index() {
            ViewBag.User = Request.Headers["X-MS-CLIENT-PRINCIPAL-NAME"];
            ViewBag.Devices = _device.GetDevicesAsync();
            return View();
        }

        public async Task<IActionResult> Color(string id, string color) {
            await _device.SetStatusLedColorAsync(id, color);
            return Redirect("/");
        }

    }

}