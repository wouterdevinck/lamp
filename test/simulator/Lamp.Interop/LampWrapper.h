#pragma once

#include "Lamp.h"
#include "AbstractPlatform.h"

using namespace lamp;

namespace LampInterop {

	public ref class LampWrapper {

	public:
		
		LampWrapper(AbstractPlatform^ platform) {
			m_Impl = new Lamp(platform->Native());
		}
		
		void Start() {
			m_Impl->start();
		}

	private:
		Lamp* m_Impl;

	};

}
