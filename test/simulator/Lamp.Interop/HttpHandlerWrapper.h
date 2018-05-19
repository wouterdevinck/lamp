#pragma once

#include <gcroot.h>
#include <msclr\marshal_cppstd.h>
#include "IHttpHandler.h"

using namespace System;
using namespace lamp;
using namespace msclr::interop;

namespace LampInterop {

	public ref class HttpHandlerWrapper { 

	private:
		IHttpHandler* pUnmanaged;

	public:
		HttpHandlerWrapper(IHttpHandler* handler) : pUnmanaged(handler) { }
		~HttpHandlerWrapper() { this->!HttpHandlerWrapper(); }
		!HttpHandlerWrapper() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IHttpHandler* Native() { return pUnmanaged; }

		String^ HandleHttpRequest(String^ method, String^ path) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("HttpHandlerWrapper");
			string m = marshal_as<string>(method);
			string p = marshal_as<string>(path);
			return gcnew String(pUnmanaged->handleHttpRequest(m, p).c_str());
		} 

	};

}