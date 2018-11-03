#pragma once

#include "IIrHandler.h"

using namespace System;
using namespace lamp;

namespace LampInterop {

	public ref class IrHandlerWrapper {

	private:
		IIrHandler* pUnmanaged;

	public:
		IrHandlerWrapper(IIrHandler* handler) : pUnmanaged(handler) {}
		~IrHandlerWrapper() { this->!IrHandlerWrapper(); }
		!IrHandlerWrapper() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IIrHandler* Native() { return pUnmanaged; }

		void HandleIrCommand(int code) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("IrHandlerWrapper");
			pUnmanaged->handleIrCommand(code);
		}

	};

}