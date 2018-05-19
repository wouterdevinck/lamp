#pragma once

#include <gcroot.h>
#include "IHttpServer.h"
#include "HttpHandlerWrapper.h"

using namespace System;
using namespace lamp;

namespace LampInterop {

	ref class AbstractHttpServer;

	public class NativeHttpServer : public IHttpServer {

	public:
		NativeHttpServer(AbstractHttpServer ^owner) : m_owner(owner) { }

	protected:
		void start(int port, IHttpHandler* handler) override;
		void stop() override;

	private:
		gcroot<AbstractHttpServer^> m_owner;

	};

	public ref class AbstractHttpServer abstract {

	private:
		IHttpServer* pUnmanaged;

	public:
		AbstractHttpServer() { pUnmanaged = new NativeHttpServer(this); }
		~AbstractHttpServer() { this->!AbstractHttpServer(); }
		!AbstractHttpServer() {
			if (pUnmanaged) {
				delete pUnmanaged;
				pUnmanaged = 0;
			}
		}
		IHttpServer* Native() { return pUnmanaged; }

	protected:
		virtual void Start(int port, HttpHandlerWrapper^ handler) = 0;
		virtual void Stop() = 0;

	internal:
		void CallStart(int port, HttpHandlerWrapper^ handler) {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractHttpServer");
			Start(port, handler);
		}
		void CallStop() {
			if (!pUnmanaged) throw gcnew ObjectDisposedException("AbstractHttpServer");
			Stop();
		}

	};

}

