#pragma once

#include <gcroot.h>
#include "IIotClient.h"

using namespace System;
using namespace std;

namespace LampInterop {

	ref class AbstractIotClient;

	public class NativeIotClient : public IIotClient {

	public:
		NativeIotClient(AbstractIotClient ^owner) : m_owner(owner) {}

	protected:
		

	private:
		gcroot<AbstractIotClient^> m_owner;

	};

	public ref class AbstractIotClient abstract {

	private:
		IIotClient* pUnmanaged;

	public:
		AbstractIotClient() { pUnmanaged = new NativeIotClient(this); }
		~AbstractIotClient() { this->!AbstractIotClient(); }
		!AbstractIotClient() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IIotClient* Native() { return pUnmanaged; }

	protected:
		

	internal:


	};

}

