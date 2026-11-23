#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>
#include "NetworkUtilsP.h"
#include "MessageClass.h"
#include <thread>

#include "MessegesOutMutexClass.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27015
#define DEFAULT_PORT_UDP DEFAULT_PORT + 1

using namespace std;

int startServer()
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0)
    {
        std::cout << "WSAStartup failed: " << result << '\n';
        return 1;
    }

    SOCKET lisSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    string local_ip = getLocalIp();
    std::cout << "Your local IP: " << local_ip << '\n';

    sockaddr_in service{};
    service.sin_family = AF_INET;
    service.sin_port = htons(DEFAULT_PORT);
    service.sin_port = htons(DEFAULT_PORT);
    service.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, local_ip.c_str(), &service.sin_addr);

    if (bind(lisSocket,
        (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
        closesocket(lisSocket);
        WSACleanup();
        return 1;
    }

    SOCKET clientSocket;
    wprintf(L"Waiting for client to connect...\n");

    listen(lisSocket, SOMAXCONN);
    
    clientSocket = accept(lisSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
        closesocket(lisSocket);
        WSACleanup();
        return 1;
    }
    else
        wprintf(L"Client connected.\n");

    Messege messege;
    MessegesMutex mmOut;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    std::cout << ">> ";
    thread tMessege(messegesGetter, ref(clientSocket), ref(messege), &mmOut);
    thread rMessege(messegesReader, ref(clientSocket), ref(messege), &mmOut);

    while (true)
    {
        string temp = mmOut.pop();
        cout << temp;

        if (temp[0] == '\x1A')
        {
            break;
        }
    }

    tMessege.detach();
    rMessege.detach();
    
    closesocket(clientSocket);
    closesocket(lisSocket);

    WSACleanup();
    return 0;
}