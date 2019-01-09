using LampInterop;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Threading;

namespace Lamp.Simulator {

    internal class Updater : AbstractUpdater {

        private string _runningHash = ComputeSha256("fpga-version-1");
        private string _installedHash = ComputeSha256("fpga-version-2");

        protected override string GetRunningVersion() {
            return FileVersionInfo.GetVersionInfo(Assembly.GetExecutingAssembly().Location).FileVersion;
        }

        protected override string GetInstalledFpgaHash() {
            return _installedHash;
        }

        protected override string GetRunningFpgaHash() {
            return _runningHash;
        }
        
        protected override bool BeginUpgrade() {
            Thread.Sleep(100);
            return true;
        }

        protected override bool WriteChunk(List<byte> chunk) {
            Thread.Sleep(1000);
            return true;
        }

        protected override bool CompleteUpgrade() {
            Thread.Sleep(500);
            return true;
        }

        protected override bool FlashFpga() {
            _runningHash = _installedHash;
            return true;
        }

        protected override UInt16 GetPreferredChunkSize() {
            return 50 * 1024;
        }
        
        private static string ComputeSha256(string input) {
            var crypt = new SHA256Managed();
            var hash = new StringBuilder();
            var crypto = crypt.ComputeHash(Encoding.UTF8.GetBytes(input));
            foreach (byte val in crypto) {
                hash.Append(val.ToString("x2"));
            }
            return hash.ToString();
        }

    }

}