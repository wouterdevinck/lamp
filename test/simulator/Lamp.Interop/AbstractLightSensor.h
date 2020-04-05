#pragma once

#include <gcroot.h>
#include "ILightSensor.h"

using namespace System;
using namespace lamp;
using namespace std;

namespace LampInterop {

	ref class AbstractLightSensor;

	public class NativeLightSensor : public ILightSensor {

	public:
		NativeLightSensor(AbstractLightSensor ^owner) : m_owner(owner) { }

	protected:
		uint32_t getIlluminanceInLux() override;

	private:
		gcroot<AbstractLightSensor^> m_owner;

	};

	public ref class AbstractLightSensor abstract {

	private:
		ILightSensor* pUnmanaged;

	public:
		AbstractLightSensor() { pUnmanaged = new NativeLightSensor(this); }
		~AbstractLightSensor() { this->!AbstractLightSensor(); }
		!AbstractLightSensor() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		ILightSensor* Native() { return pUnmanaged; }

	protected:
		virtual UInt32 GetIlluminanceInLux() = 0;

	internal:
		UInt32 CallGetIlluminanceInLux() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLightSensor");
			return GetIlluminanceInLux();
		}

	};

}

