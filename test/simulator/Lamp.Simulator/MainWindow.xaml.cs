using LampInterop;

namespace Lamp.Simulator {

    public partial class MainWindow {

        private const int Port = 8282;

        public MainWindow() {
            InitializeComponent();

            var updater = new Updater();
            var logger = new Logger();
            var httpserver = new HttpServer();
            var httpclient = new HttpClient();
            var led = new RgbLed(ellipse);
            var leds = new LedArray();

            var lamp = new LampWrapper(updater, logger, httpserver, httpclient, led, leds); 
            
            lamp.Start(Port);

            // Browser
            browser.Navigated += (s, e) => {
                url.Text = browser.Source?.ToString();
                btnBack.IsEnabled = browser.CanGoBack;
                btnNext.IsEnabled = browser.CanGoForward;
            };
            browser.Navigate("http://localhost:" + Port);
            btnBack.Click += (s, e) => browser.GoBack();
            btnNext.Click += (s, e) => browser.GoForward();

            // Logs
            log.DataContext = logger;
            logger.PropertyChanged += (s, e) =>
                Dispatcher.Invoke(() => 
                    logscroll.ScrollToBottom());

        }

    }

}