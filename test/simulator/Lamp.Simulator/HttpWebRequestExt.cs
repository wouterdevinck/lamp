using System.Net;

namespace Lamp.Simulator {

    public static class HttpWebResponseExt {

        public static HttpWebResponse GetResponseNoException(this WebRequest req) {
            try {
                return (HttpWebResponse)req.GetResponse();
            } catch (WebException we) {
                var resp = we.Response as HttpWebResponse;
                if (resp == null) throw;
                return resp;
            }
        }

    }

}