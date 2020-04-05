#pragma once

#include <gcroot.h>
#include "IPlatform.h"
#include "AbstractIrReceiver.h"
#include "AbstractLedBoardChain.h"
#include "AbstractHttpClient.h"
#include "AbstractHttpServer.h"
#include "AbstractLogger.h"
#include "AbstractUpdater.h"
#include "AbstractRgbLed.h"
#include "AbstractNvs.h"
#include "AbstractWiFiClient.h"
#include "AbstractIotFramework.h"
#include "AbstractLightSensor.h"

using namespace lamp;

namespace LampInterop {

	ref class AbstractPlatform;

	public class NativePlatform : public IPlatform {

	public:
		NativePlatform(AbstractPlatform ^owner) : m_owner(owner) {}

	protected:
		IIrReceiver* getIrReceiver() override;
		IRgbLed* getRgbLed() override;
		ILedBoardChain* getLedBoardChain() override;
		IWiFiClient* getWiFiClient() override;
		IUpdater* getUpdater() override;
		ILogger* getLogger() override;
		IHttpServer* getHttpServer() override;
		IHttpClient* getHttpClient() override;
		IIotFramework* getIotFramework() override;
		INvs* getStorage() override;
		ILightSensor* getLightSensor() override;
		int getHttpServerPort() override;

	private:
		gcroot<AbstractPlatform^> m_owner;

	};

	public ref class AbstractPlatform abstract {

	private:
		IPlatform* pUnmanaged;

	public:
		AbstractPlatform() { pUnmanaged = new NativePlatform(this); }
		~AbstractPlatform() { this->!AbstractPlatform(); }
		!AbstractPlatform() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IPlatform* Native() { return pUnmanaged; }

	protected:
		virtual AbstractIrReceiver^ GetIrReceiver() = 0;
		virtual AbstractRgbLed^ GetRgbLed() = 0;
		virtual AbstractLedBoardChain^ GetLedBoardChain() = 0;
		virtual AbstractWiFiClient^ GetWiFiClient() = 0;
		virtual AbstractUpdater^ GetUpdater() = 0;
		virtual AbstractLogger^ GetLogger() = 0;
		virtual AbstractHttpServer^ GetHttpServer() = 0;
		virtual AbstractHttpClient^ GetHttpClient() = 0;
		virtual AbstractIotFramework^ GetIotFramework() = 0;
		virtual AbstractNvs^ GetStorage() = 0;
		virtual AbstractLightSensor^ GetLightSensor() = 0;
		virtual int GetHttpServerPort() = 0;

	internal:
		IIrReceiver* CallGetIrReceiver() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetIrReceiver()->Native();
		}
		IRgbLed* CallGetRgbLed() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetRgbLed()->Native();
		}
		ILedBoardChain* CallGetLedBoardChain() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetLedBoardChain()->Native();
		}
		IWiFiClient* CallGetWiFiClient() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetWiFiClient()->Native();
		}
		IUpdater* CallGetUpdater() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetUpdater()->Native();
		}
		ILogger* CallGetLogger() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetLogger()->Native();
		}
		IHttpServer* CallGetHttpServer() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetHttpServer()->Native();
		}
		IHttpClient* CallGetHttpClient() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetHttpClient()->Native();
		}
		IIotFramework* CallGetIotFramework() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetIotFramework()->Native();
		}
		INvs* CallGetStorage() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetStorage()->Native();
		}
		ILightSensor* CallGetLightSensor() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetLightSensor()->Native();
		}
		int CallGetHttpServerPort() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractPlatform");
			return GetHttpServerPort();
		}

	};

}