using Microsoft.AspNetCore.Http;
using System.Collections.Generic;
using System.Net;
using System.Threading.Tasks;

namespace LampControl {

    public class WhitelistMiddleware {

        private readonly List<string> _userWhitelist = new List<string>() {
            "b83ffb1a4d2e0f4b" // wouter.devinck@belgacom.net
        };

        private readonly RequestDelegate _next;

        public WhitelistMiddleware(RequestDelegate next) {
            _next = next;
        }

        public async Task InvokeAsync(HttpContext context) {
            if (context.Request.Host.Host == "localhost") { 
                await _next(context);
            } else {
                var pid = context.Request.Headers["X-MS-CLIENT-PRINCIPAL-ID"];
                if (string.IsNullOrEmpty(pid)) {
                    context.Response.StatusCode = (int)HttpStatusCode.Unauthorized;
                    await context.Response.WriteAsync("User ID not received");
                } else {
                    if (!_userWhitelist.Contains(pid)) {
                        context.Response.StatusCode = (int)HttpStatusCode.Unauthorized;
                        await context.Response.WriteAsync("User not whitelisted");
                    } else {
                        await _next(context);
                    }
                }
            }
        }

    }

}