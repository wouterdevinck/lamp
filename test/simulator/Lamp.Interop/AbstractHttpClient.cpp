#include "stdafx.h"
#include "AbstractHttpClient.h"

using namespace LampInterop;

HttpResponse NativeHttpClient::request(HttpRequest req) {
	return m_owner->CallRequest(req);
}
