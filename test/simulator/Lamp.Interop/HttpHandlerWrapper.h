#pragma once

#include "IHttpHandler.h"
#include "MarshalHelper.h"

using namespace System;
using namespace lamp;

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

		String^ HandleHttpRequest(String^ method, String^ path, String^ query, String^ body) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("HttpHandlerWrapper");
			string m = MarshalHelper::Convert(method);
			string p = MarshalHelper::Convert(path);
			string q = MarshalHelper::Convert(query);
			string b = MarshalHelper::Convert(body);
			string r = pUnmanaged->handleHttpRequest(m, p, q, b);
			return MarshalHelper::Convert(r);
		} 

	};

}