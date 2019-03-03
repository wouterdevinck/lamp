#pragma once

#include <gcroot.h>
#include "INvs.h"
#include "MarshalHelper.h"

using namespace System;
using namespace lamp;
using namespace std;

namespace LampInterop {

	ref class AbstractNvs;

	public class NativeNvs : public INvs {

	public:
		NativeNvs(AbstractNvs ^owner) : m_owner(owner) {}

	protected:
		string getValue(string key) override;
		void setValue(string key, string value) override;

	private:
		gcroot<AbstractNvs^> m_owner;

	};

	public ref class AbstractNvs abstract {

	private:
		INvs* pUnmanaged;

	public:
		AbstractNvs() { pUnmanaged = new NativeNvs(this); }
		~AbstractNvs() { this->!AbstractNvs(); }
		!AbstractNvs() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		INvs* Native() { return pUnmanaged; }

	protected:
		virtual String^ GetValue(String^ key) = 0;
		virtual void SetValue(String^ key, String^ value) = 0;

	internal:
		string CallGetValue(string key) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractNvs");
			return MarshalHelper::Convert(GetValue(MarshalHelper::Convert(key)));
		}
		void CallSetValue(string key, string value) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractNvs");
			SetValue(MarshalHelper::Convert(key), MarshalHelper::Convert(value));
		}

	};

}

