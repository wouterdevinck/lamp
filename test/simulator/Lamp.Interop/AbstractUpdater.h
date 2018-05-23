#pragma once

#include <gcroot.h>
#include "IUpdater.h"

using namespace System;
using namespace lamp;
using namespace std;

namespace LampInterop {

	ref class AbstractUpdater;

	public class NativeUpdater : public IUpdater {

	public:
		NativeUpdater(AbstractUpdater ^owner) : m_owner(owner) {}

	protected:
		

	private:
		gcroot<AbstractUpdater^> m_owner;

	};

	public ref class AbstractUpdater abstract {

	private:
		IUpdater* pUnmanaged;

	public:
		AbstractUpdater() { pUnmanaged = new NativeUpdater(this); }
		~AbstractUpdater() { this->!AbstractUpdater(); }
		!AbstractUpdater() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IUpdater* Native() { return pUnmanaged; }

	protected:
		

	internal:
		

	};

}

