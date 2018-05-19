#pragma once

#include <gcroot.h>
#include "IRgbLed.h"

using namespace System;
using namespace lamp;
using namespace std;

namespace LampInterop {

	ref class AbstractRgbLed;

	public ref struct RgbLedColorWrapper {

		RgbLedColorWrapper(RgbLedColor color) {
			R = color.r;
			G = color.g;
			B = color.b;
		}

		property Byte R;
		property Byte G;
		property Byte B;

	};

	public class NativeRgbLed : public IRgbLed {

	public:
		NativeRgbLed(AbstractRgbLed ^owner) : m_owner(owner) { }

	protected:
		void setLedColor(RgbLedColor color) override;

	private:
		gcroot<AbstractRgbLed^> m_owner;

	};

	public ref class AbstractRgbLed abstract {

	private:
		IRgbLed* pUnmanaged;

	public:
		AbstractRgbLed() { pUnmanaged = new NativeRgbLed(this); }
		~AbstractRgbLed() { this->!AbstractRgbLed(); }
		!AbstractRgbLed() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IRgbLed* Native() { return pUnmanaged; }

	protected:
		virtual void SetLedColor(RgbLedColorWrapper^ color) = 0;

	internal:
		void CallSetLedColor(RgbLedColor color) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractRgbLedChain");
			auto wrapper = gcnew RgbLedColorWrapper(color);
			SetLedColor(wrapper);
		}

	};

}

