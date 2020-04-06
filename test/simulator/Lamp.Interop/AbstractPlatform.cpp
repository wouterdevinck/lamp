#include "stdafx.h"
#include "AbstractPlatform.h"

using namespace LampInterop;

IIrReceiver* NativePlatform::getIrReceiver() {
	return m_owner->CallGetIrReceiver();
}

IRgbLed* NativePlatform::getRgbLed() {
	return m_owner->CallGetRgbLed();
}

ILedBoardChain* NativePlatform::getLedBoardChain() {
	return m_owner->CallGetLedBoardChain();
}

IWiFiClient* NativePlatform::getWiFiClient() {
	return m_owner->CallGetWiFiClient();
}

IUpdater* NativePlatform::getUpdater() {
	return m_owner->CallGetUpdater();
}

ILogger* NativePlatform::getLogger() {
	return m_owner->CallGetLogger();
}

IHttpServer* NativePlatform::getHttpServer() {
	return m_owner->CallGetHttpServer();
}

IHttpClient* NativePlatform::getHttpClient() {
	return m_owner->CallGetHttpClient();
}

IIotFramework * NativePlatform::getIotFramework() {
	return m_owner->CallGetIotFramework();
}

INvs* NativePlatform::getStorage() {
	return m_owner->CallGetStorage();
}

ILightSensor* NativePlatform::getLightSensor() {
	return m_owner->CallGetLightSensor();
}

int NativePlatform::getHttpServerPort() {
	return m_owner->CallGetHttpServerPort();
}
