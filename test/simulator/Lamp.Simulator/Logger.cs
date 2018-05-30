using System;
using LampInterop;
using System.ComponentModel;

namespace Lamp.Simulator {

    internal class Logger : AbstractLogger, INotifyPropertyChanged {

        public string Log { get; private set; }

        public event PropertyChangedEventHandler PropertyChanged;

        private void NotifyPropertyChanged(string propertyName = "") {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void LogLine(string severity, string tag, string line) {
            Log += $"[{severity}] {tag}: {line}" + Environment.NewLine;
            NotifyPropertyChanged("Log");
        }
        
        protected override void LogError(string tag, string line) {
            LogLine("ERROR", tag, line);
        }

        protected override void LogWarning(string tag, string line) {
            LogLine("WARNING", tag, line);
        }

        protected override void LogInfo(string tag, string line) {
            LogLine("INFO", tag, line);
        }

        protected override void LogDebug(string tag, string line) {
            LogLine("DEBUG", tag, line);
        }

        protected override void LogVerbose(string tag, string line) {
            LogLine("VERBOSE", tag, line);
        }
        
    }

}