#pragma once

#include "IIotHandler.h"
#include "MarshalHelper.h"

using namespace System;

namespace LampInterop {

	public ref class IotHandlerWrapper {

	private:
		IIotHandler* pUnmanaged;

	public:
		IotHandlerWrapper(IIotHandler* handler) : pUnmanaged(handler) {}
		~IotHandlerWrapper() { this->!IotHandlerWrapper(); }
		!IotHandlerWrapper() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IIotHandler* Native() { return pUnmanaged; }

		String^ HandleDirectMethod(String^ method, String^ payload) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("IotHandlerWrapper");
			string m = MarshalHelper::Convert(method);
			string p = MarshalHelper::Convert(payload);
			string resp = pUnmanaged->handleDirectMethod(m, p);
			return MarshalHelper::Convert(resp);
		}

	};

}