#pragma once

#include <gcroot.h>
#include "IHttpClient.h"
#include "MarshalHelper.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace lamp;
using namespace std;

namespace LampInterop {

	ref class AbstractHttpClient;

	public ref struct HttpRequestWrapper {

		HttpRequestWrapper(HttpRequest req) {
			Method = MarshalHelper::Convert(req.method);
			Url = MarshalHelper::Convert(req.url);
			Headers = MarshalHelper::Convert(req.headers);
		}

		property String^ Method;
		property String^ Url;
		property Dictionary<String^, String^>^ Headers;

	};

	public ref struct HttpResponseWrapper {

		property Byte Status;
		property Dictionary<String^, String^>^ Headers;
		property List<Byte>^ Body;
		
		property HttpResponse Native { 
			HttpResponse get() {
				return HttpResponse {
					Status,
					MarshalHelper::Convert(Headers),
					MarshalHelper::Convert(Body)
				};
			} 
		};

	};

	public class NativeHttpClient : public IHttpClient {

	public:
		NativeHttpClient(AbstractHttpClient ^owner) : m_owner(owner) {}

	protected:
		HttpResponse request(HttpRequest req) override;

	private:
		gcroot<AbstractHttpClient^> m_owner;

	};

	public ref class AbstractHttpClient abstract {

	private:
		IHttpClient* pUnmanaged;

	public:
		AbstractHttpClient() { pUnmanaged = new NativeHttpClient(this); }
		~AbstractHttpClient() { this->!AbstractHttpClient(); }
		!AbstractHttpClient() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IHttpClient* Native() { return pUnmanaged; }

	protected:
		virtual HttpResponseWrapper^ Request(HttpRequestWrapper^ req) = 0;

	internal:
		HttpResponse CallRequest(HttpRequest req) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractRgbLedChain");
			auto wrapper = gcnew HttpRequestWrapper(req);
			return Request(wrapper)->Native;
		}

	};

}

