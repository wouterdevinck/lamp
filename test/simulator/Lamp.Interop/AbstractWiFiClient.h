#pragma once

#include <gcroot.h>
#include "IWiFiClient.h"
#include "MarshalHelper.h"
#include "WiFiHandlerWrapper.h"

using namespace System;
using namespace lamp;
using namespace std;

namespace LampInterop {
	
	ref class WiFiHandlerWrapper;
	ref class AbstractWiFiClient;

	public class NativeWiFiClient : public IWiFiClient {

	public:
		NativeWiFiClient(AbstractWiFiClient ^owner) : m_owner(owner) {}

	protected:
		void setHandler(IWiFiHandler* handler) override;
		void connect(string ssid, string pwd) override;
		void reconnect() override;
		void startSmartConfig() override;

	private:
		gcroot<AbstractWiFiClient^> m_owner;

	};

	public ref class AbstractWiFiClient abstract {

	private:
		IWiFiClient* pUnmanaged;

	public:
		AbstractWiFiClient() { pUnmanaged = new NativeWiFiClient(this); }
		~AbstractWiFiClient() { this->!AbstractWiFiClient(); }
		!AbstractWiFiClient() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IWiFiClient* Native() { return pUnmanaged; }

	protected:
		virtual void SetHandler(WiFiHandlerWrapper^ handler) = 0;
		virtual void Connect(String^ ssid, String^ pwd) = 0;
		virtual void Reconnect() = 0;
		virtual void StartSmartConfig() = 0;

	internal:
		void CallSetHandler(IWiFiHandler* handler) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractWiFiClient");
			auto h = gcnew WiFiHandlerWrapper(handler);
			SetHandler(h);
		}
		void CallConnect(string ssid, string pwd) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractWiFiClient");
			Connect(MarshalHelper::Convert(ssid), MarshalHelper::Convert(pwd));
		}
		void CallReconnect() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractWiFiClient");
			Reconnect();
		}
		void CallStartSmartConfig() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractWiFiClient");
			StartSmartConfig();
		}

	};

}

