#pragma once

#include <gcroot.h>
#include "IUpdater.h"
#include "MarshalHelper.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace lamp;
using namespace std;

namespace LampInterop {

	ref class AbstractUpdater;

	public class NativeUpdater : public IUpdater {

	public:
		NativeUpdater(AbstractUpdater ^owner) : m_owner(owner) {}

	protected:
		string getRunningVersion() override;
		string getRunningFpgaHash() override;
		string getInstalledFpgaHash() override;
		bool beginUpgrade() override;
		bool writeChunk(vector<uint8_t> chunk) override;
		bool completeUpgrade() override;
		bool flashFpga() override;   
		uint16_t getPreferredChunkSize() override;

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
		virtual String^ GetRunningVersion() = 0;
		virtual String^ GetRunningFpgaHash() = 0;
		virtual String^ GetInstalledFpgaHash() = 0;
		virtual Boolean BeginUpgrade() = 0;
		virtual Boolean WriteChunk(List<Byte>^ chunk) = 0;
		virtual Boolean CompleteUpgrade() = 0;
		virtual Boolean FlashFpga() = 0;
		virtual UInt16 GetPreferredChunkSize() = 0;

	internal:
		string CallGetRunningVersion() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractUpdater");
			return MarshalHelper::Convert(GetRunningVersion());
		}
		string CallGetRunningFpgaHash() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractUpdater");
			return MarshalHelper::Convert(GetRunningFpgaHash());
		}
		string CallGetInstalledFpgaHash() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractUpdater");
			return MarshalHelper::Convert(GetInstalledFpgaHash());
		}
		bool CallBeginUpgrade() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractUpdater");
			return BeginUpgrade();
		}
		bool CallWriteChunk(vector<uint8_t> chunk) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractUpdater");
			return WriteChunk(MarshalHelper::Convert(chunk));
		}
		bool CallCompleteUpgrade() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractUpdater");
			return CompleteUpgrade();
		}
		bool CallFlashFpga() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractUpdater");
			return FlashFpga();
		}
		UInt16 CallGetPreferredChunkSize() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractUpdater");
			return GetPreferredChunkSize();
		}
		
	};

}