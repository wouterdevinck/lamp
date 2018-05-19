using LampInterop;
using System;
using System.Net;
using System.Text;
using System.Threading;

namespace Lamp.Simulator {

    internal class HttpServer : AbstractHttpServer {

        private HttpListener _listener;

        public HttpServer() {
            if (!HttpListener.IsSupported) {
                throw new NotSupportedException("HttpListener needs Windows XP SP2, Server 2003 or later.");
            }
        }

        protected override void Start(int port, HttpHandlerWrapper handler) {
            if (_listener != null && _listener.IsListening) {
                Stop();
            }
            _listener = new HttpListener();
            _listener.Prefixes.Add($"http://localhost:{port}/");
            _listener.Start();
            ThreadPool.QueueUserWorkItem(_ => {
                try {
                    while (_listener.IsListening) {
                        ThreadPool.QueueUserWorkItem(c => {
                            if (!(c is HttpListenerContext ctx)) return;
                            try {
                                var rstr = handler.HandleHttpRequest(ctx.Request.HttpMethod, ctx.Request.RawUrl);
                                var buf = Encoding.UTF8.GetBytes(rstr);
                                ctx.Response.ContentLength64 = buf.Length;
                                ctx.Response.OutputStream.Write(buf, 0, buf.Length);
                            } catch {
                                // ignored
                            } finally {
                                ctx.Response.OutputStream.Close();
                            }
                        }, _listener.GetContext());
                    }
                } catch {
                    // ignored
                }
            });
        }

        protected override void Stop() {
            _listener.Stop();
            _listener.Close();
        }

    }

}