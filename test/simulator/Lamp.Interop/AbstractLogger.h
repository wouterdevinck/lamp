#pragma once

#include <gcroot.h>
#include "ILogger.h"
#include "MarshalHelper.h"

using namespace System;
using namespace lamp;
using namespace std;

namespace LampInterop {

	ref class AbstractLogger;

	public class NativeLogger : public ILogger {

	public:
		NativeLogger(AbstractLogger ^owner) : m_owner(owner) {}

	protected:
		void logError(string tag, string line) override;
		void logWarning(string tag, string line) override;
		void logInfo(string tag, string line) override;
		void logDebug(string tag, string line) override;
		void logVerbose(string tag, string line) override;

	private:
		gcroot<AbstractLogger^> m_owner;

	};

	public ref class AbstractLogger abstract {

	private:
		ILogger* pUnmanaged;

	public:
		AbstractLogger() { pUnmanaged = new NativeLogger(this); }
		~AbstractLogger() { this->!AbstractLogger(); }
		!AbstractLogger() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		ILogger* Native() { return pUnmanaged; }

	protected:
		virtual void LogError(String^ tag, String^ line) = 0;
		virtual void LogWarning(String^ tag, String^ line) = 0;
		virtual void LogInfo(String^ tag, String^ line) = 0;
		virtual void LogDebug(String^ tag, String^ line) = 0;
		virtual void LogVerbose(String^ tag, String^ line) = 0;

	internal:
		void CallLogError(string tag, string line) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLogger");
			LogError(MarshalHelper::Convert(tag), MarshalHelper::Convert(line));
		}
		void CallLogWarning(string tag, string line) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLogger");
			LogWarning(MarshalHelper::Convert(tag), MarshalHelper::Convert(line));
		}
		void CallLogInfo(string tag, string line) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLogger");
			LogInfo(MarshalHelper::Convert(tag), MarshalHelper::Convert(line));
		}
		void CallLogDebug(string tag, string line) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLogger");
			LogDebug(MarshalHelper::Convert(tag), MarshalHelper::Convert(line));
		}
		void CallLogVerbose(string tag, string line) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractLogger");
			LogVerbose(MarshalHelper::Convert(tag), MarshalHelper::Convert(line));
		}

	};

}

