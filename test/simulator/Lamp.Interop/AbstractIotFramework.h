#pragma once

#include <gcroot.h>
#include "IIotFramework.h"
#include "IotHandlerWrapper.h"
#include "AbstractIotClient.h"

using namespace System;
using namespace std;

namespace LampInterop {
	
	ref class IotHandlerWrapper;
	ref class AbstractIotFramework;

	public class NativeIotFramework : public IIotFramework {

	public:
		NativeIotFramework(AbstractIotFramework ^owner) : m_owner(owner) {}

	protected:
		IIotClient* connect(IIotHandler* handler) override;

	private:
		gcroot<AbstractIotFramework^> m_owner;

	};

	public ref class AbstractIotFramework abstract {

	private:
		IIotFramework* pUnmanaged;

	public:
		AbstractIotFramework() { pUnmanaged = new NativeIotFramework(this); }
		~AbstractIotFramework() { this->!AbstractIotFramework(); }
		!AbstractIotFramework() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IIotFramework* Native() { return pUnmanaged; }

	protected:
		virtual AbstractIotClient^ Connect(IotHandlerWrapper^ handler) = 0;

	internal:
		IIotClient* CallConnect(IIotHandler* handler) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractIotFramework");
			auto h = gcnew IotHandlerWrapper(handler);
			auto resp = Connect(h);
			return resp->Native(); 
		}

	};

}
