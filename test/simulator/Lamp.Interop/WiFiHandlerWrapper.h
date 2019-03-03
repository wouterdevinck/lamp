#pragma once

#include "IWiFiHandler.h"
#include "MarshalHelper.h"

using namespace System;
using namespace lamp;

namespace LampInterop {

	public ref class WiFiHandlerWrapper {

	private:
		IWiFiHandler* pUnmanaged;

	public:
		WiFiHandlerWrapper(IWiFiHandler* handler) : pUnmanaged(handler) {}
		~WiFiHandlerWrapper() { this->!WiFiHandlerWrapper(); }
		!WiFiHandlerWrapper() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IWiFiHandler* Native() { return pUnmanaged; }
		
		void OnConnected() {
			pUnmanaged->onConnected();
		}

		void OnDisconnected() {
			pUnmanaged->onDisconnected();
		}

		void OnSmartConfig(String^ ssid, String^ password) {
			string s = MarshalHelper::Convert(ssid);
			string p = MarshalHelper::Convert(password);
			pUnmanaged->onSmartConfig(s, p);
		}

	};

}