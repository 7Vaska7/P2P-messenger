#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <future>

#include "MessageClass.h"
#include "NetworkUtilsP.h"
#include "ServerUtils.h"
#include "MessegesOutMutexClass.h"

#pragma comment(lib, "Ws2_32.lib")

inline constexpr uint16_t DEFAULT_PORT = 27015;
inline constexpr uint16_t DEFAULT_PORT_UDP = DEFAULT_PORT + 1;

using namespace std;

int startServer(MessegesMutex& mmOut, MessegesMutex& mmIn, promise<void> clientPromise)
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0)
    {
        std::cout << "WSAStartup failed: " << result << '\n';
        return 1;
    }

    thread ipReturnerT(&ServerUtils::waitReturnIP, DEFAULT_PORT_UDP);

    SOCKET lisSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    string local_ip = getLocalIp();
    std::cout << "Your local IP: " << local_ip << '\n';

    sockaddr_in service{};
    service.sin_family = AF_INET;
    service.sin_port = htons(DEFAULT_PORT);
    service.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, local_ip.c_str(), &service.sin_addr);

    if (::bind(lisSocket,
        (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
        closesocket(lisSocket);
        WSACleanup();
        return 1;
    }

    clientPromise.set_value();

    while (true)
    {
        
    }
    
    closesocket(lisSocket);

    WSACleanup();
    return 0;
}