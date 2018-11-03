#pragma once

#include <gcroot.h>
#include "IIrReceiver.h"
#include "IrHandlerWrapper.h"

using namespace System;
using namespace lamp;

namespace LampInterop {

	ref class AbstractIrReceiver;

	public class NativeIrReceiver : public IIrReceiver {

	public:
		NativeIrReceiver(AbstractIrReceiver ^owner) : m_owner(owner) {}

	protected:
		void start(IIrHandler* handler) override;

	private:
		gcroot<AbstractIrReceiver^> m_owner;

	};

	public ref class AbstractIrReceiver abstract {

	private:
		IIrReceiver* pUnmanaged;

	public:
		AbstractIrReceiver() { pUnmanaged = new NativeIrReceiver(this); }
		~AbstractIrReceiver() { this->!AbstractIrReceiver(); }
		!AbstractIrReceiver() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IIrReceiver* Native() { return pUnmanaged; }

	protected:
		virtual void Start(IrHandlerWrapper^ handler) = 0;

	internal:
		void CallStart(IIrHandler* handler) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractIrReceiver");
			auto h = gcnew IrHandlerWrapper(handler);
			Start(h);
		}

	};

}

