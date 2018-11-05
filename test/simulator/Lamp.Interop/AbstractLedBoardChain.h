#pragma once

#include <gcroot.h>
#include "ILedBoardChain.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace lamp;
using namespace std;

namespace LampInterop {

	ref class AbstractLedBoardChain;

	public ref struct LedValueWrapper {

		LedValueWrapper(LedValue value) {
			R = value.r;
			G = value.g;
			B = value.b;
			W = value.w;
		}

		property UInt16 R;
		property UInt16 G;
		property UInt16 B;
		property UInt16 W;

	};

	public ref struct ChainInfoWrapper {

		property Byte Channels;
		property Byte Drivers;

		property ChainInfo Native {
			ChainInfo get() {
				return ChainInfo {
					Channels,
					Drivers
				};
			}
		};

	};

	public ref struct KeyFrameWrapper {

		KeyFrameWrapper(KeyFrame value) {
			Duration = TimeSpan::FromMilliseconds((Double)value.duration.count());
			Type = value.type;
			Frame = gcnew List<LedValueWrapper^>();
			for (LedValue val : value.frame) {
				auto wrapper = gcnew LedValueWrapper(val);
				Frame->Add(wrapper);
			}
		}

		property TimeSpan^ Duration;
		property AnimationType Type;
		property List<LedValueWrapper^>^ Frame;

	};

	public class NativeLedBoardChain : public ILedBoardChain {

	public:
		NativeLedBoardChain(AbstractLedBoardChain ^owner) : m_owner(owner) {}

	protected:
		void addKeyframe(KeyFrame keyframe) override;
		void setAllLeds(LedValue color) override;
		void setAllLeds(LedValue values[]) override;
		void setBrightness(uint8_t brightness) override;
		void setBrightness(uint8_t values[]) override;
		ChainInfo getChainInfo() override;

	private:
		gcroot<AbstractLedBoardChain^> m_owner;

	};

	public ref class AbstractLedBoardChain abstract {

	private:
		ILedBoardChain* pUnmanaged;

	public:
		AbstractLedBoardChain() { pUnmanaged = new NativeLedBoardChain(this); }
		~AbstractLedBoardChain() { this->!AbstractLedBoardChain(); }
		!AbstractLedBoardChain() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		ILedBoardChain* Native() { return pUnmanaged; }

	protected:
		virtual void AddKeyframe(KeyFrameWrapper^ keyframe) = 0;
		virtual void SetAllLeds(LedValueWrapper^ color) = 0;
		virtual void SetAllLeds(List<LedValueWrapper^>^ values) = 0;
		virtual void SetBrightness(Byte brightness) = 0;
		virtual void SetBrightness(List<Byte>^ values) = 0;
		virtual ChainInfoWrapper^ GetChainInfo() = 0;

	internal:
		void CallAddKeyframe(KeyFrame keyframe) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLedBoardChain");
			auto wrapper = gcnew KeyFrameWrapper(keyframe);
			AddKeyframe(wrapper);
		}
		void CallSetAllLeds(LedValue color) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLedBoardChain");
			auto wrapper = gcnew LedValueWrapper(color);
			SetAllLeds(wrapper);
		}
		void CallSetAllLeds(LedValue values[]) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLedBoardChain");
			auto list = gcnew List<LedValueWrapper^>();
			if (sizeof(values) > 0) {
				for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
					auto wrapper = gcnew LedValueWrapper(values[i]);
					list->Add(wrapper);
				}
			}
			SetAllLeds(list);
		}
		void CallSetBrightness(uint8_t brightness) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLedBoardChain");
			SetBrightness(brightness);
		}
		void CallSetBrightness(uint8_t values[]) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLedBoardChain");
			auto list = gcnew List<Byte>();
			if (sizeof(values) > 0) {
				for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
					list->Add(values[i]);
				}
			}
			SetBrightness(list);
		}
		ChainInfo CallGetChainInfo() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLedBoardChain");
			return GetChainInfo()->Native;
		}

	};

}

