using LampInterop;

namespace Lamp.Simulator {

    public partial class MainWindow {

        private const int Port = 8282;

        public MainWindow() {
            InitializeComponent();

            var platform = new Platform(Port, ellipse, ledDisplay);
            var lamp = new LampWrapper(platform);
            remote.Ir = platform.IrReceiver;
            lamp.Start();

            // Browser
            browser.Navigated += (s, e) => {
                url.Text = browser.Source.ToString();
                btnBack.IsEnabled = browser.CanGoBack;
                btnNext.IsEnabled = browser.CanGoForward;
            };
            browser.Navigate("http://localhost:" + Port);
            btnBack.Click += (s, e) => browser.GoBack();
            btnNext.Click += (s, e) => browser.GoForward();

            // Logs
            log.DataContext = platform.Logger;
            platform.Logger.PropertyChanged += (s, e) =>
                Dispatcher.Invoke(() => 
                    logscroll.ScrollToBottom());

        }

    }

}