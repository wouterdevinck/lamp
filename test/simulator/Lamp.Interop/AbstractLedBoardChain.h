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

	public class NativeLedBoardChain : public ILedBoardChain {

	public:
		NativeLedBoardChain(AbstractLedBoardChain ^owner) : m_owner(owner) {}

	protected:
		void addKeyframe(milliseconds duration, vector<LedValue> keyframe) override;

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
		virtual void AddKeyframe(Int64 milliseconds, List<LedValueWrapper^>^ keyframe) = 0;

	internal:
		void CallAddKeyframe(milliseconds duration, vector<LedValue> keyframe) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractRgbLedChain");
			vector<LedValue>::iterator itr;
			List<LedValueWrapper^>^ list = gcnew List<LedValueWrapper^>();
			for (itr = keyframe.begin(); itr != keyframe.end(); itr++) {
				auto wrapper = gcnew LedValueWrapper(*itr);
				list->Add(wrapper);
			}
			AddKeyframe(duration.count(), list);
		}

	};

}

