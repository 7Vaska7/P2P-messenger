#pragma once

#include <winsock2.h>
#include <WS2tcpip.h>

#include "MessageClass.h"
#include "MessegesOutMutexClass.h"

#pragma comment(lib, "Ws2_32.lib")

class ServerUtils
{
private:

public:
	void messegeGetter(SOCKET& clientSocket, MessegesMutex* mmOut);
	static int waitReturnIP(int port);
};

