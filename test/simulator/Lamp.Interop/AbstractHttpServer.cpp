#include "stdafx.h"
#include "AbstractHttpServer.h"

using namespace LampInterop;

void NativeHttpServer::start(int port, IHttpHandler* handler) {
	return m_owner->CallStart(port, handler);
}

void NativeHttpServer::stop() {
	return m_owner->CallStop();
}