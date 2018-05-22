#pragma once

#include "Lamp.h"
#include "AbstractHttpServer.h"
#include "AbstractRgbLed.h"
#include "AbstractLedBoardChain.h"
#include "AbstractUpdater.h"

using namespace System;
using namespace lamp;

namespace LampInterop {

	public ref class LampWrapper {

	public:
		
		LampWrapper(AbstractUpdater^ updater, AbstractHttpServer^ http, AbstractRgbLed^ led, AbstractLedBoardChain^ leds) {
			m_Impl = new Lamp(updater->Native(), http->Native(), led->Native(), leds->Native());
		}
		
		void Start(int port) {
			m_Impl->start(port);
		}

	private:
		Lamp* m_Impl;

	};

}
