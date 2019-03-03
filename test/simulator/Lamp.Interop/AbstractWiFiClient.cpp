#include "stdafx.h"
#include "AbstractWiFiClient.h"

using namespace LampInterop;

void NativeWiFiClient::setHandler(IWiFiHandler* handler) {
	m_owner->CallSetHandler(handler);
}

void NativeWiFiClient::connect(string ssid, string pwd) {
	m_owner->CallConnect(ssid, pwd);
}

void NativeWiFiClient::reconnect() {
	m_owner->CallReconnect();
}

void NativeWiFiClient::startSmartConfig() {
	m_owner->CallStartSmartConfig();
}