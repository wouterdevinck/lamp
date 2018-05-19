#include "stdafx.h"
#include "AbstractHttpServer.h"

using namespace LampInterop;

void NativeHttpServer::start(int port, IHttpHandler* handler) {
	auto h = gcnew HttpHandlerWrapper(handler);
	return m_owner->CallStart(port, h);
}

void NativeHttpServer::stop() {
	return m_owner->CallStop();
}