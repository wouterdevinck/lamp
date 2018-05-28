using LampInterop;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;

namespace Lamp.Simulator {

    internal class HttpClient : AbstractHttpClient {
        
        protected override HttpResponseWrapper Request(HttpRequestWrapper req) {
            ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;
            var request = WebRequest.Create(req.Url);
            request.Method = req.Method;
            foreach (var header in req.Headers) {
                if (!WebHeaderCollection.IsRestricted(header.Key)) {
                    request.Headers.Add(header.Key, header.Value);
                } else {
                    if(header.Key == "Range") {
                        var one = header.Value.Split('=');
                        var two = one[1].Split('-');
                        (request as HttpWebRequest).AddRange(one[0], 
                            int.Parse(two[0]), int.Parse(two[1]));
                    } else {
                        throw new Exception("Restricted header");
                    }
                }
            }
            var resp = request.GetResponseNoException();
            var length = (int)resp.ContentLength;
            var body = new byte[0];
            if (length > 0) {
                var dataStream = resp.GetResponseStream();
                var reader = new BinaryReader(dataStream);
                body = reader.ReadBytes(length);
                reader.Close();
            }
            resp.Close();
            var headers = new Dictionary<string, string>();
            foreach (var header in resp.Headers.AllKeys) {
                headers.Add(header, resp.Headers[header]);
            }
            return new HttpResponseWrapper() {
                Status = (byte)resp.StatusCode,
                Headers = headers,
                Body = new List<byte>(body)
            };
        }

    }

}