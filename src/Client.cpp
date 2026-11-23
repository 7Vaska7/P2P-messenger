#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>
#include <limits>
#include <thread>

#include "NetworkUtilsP.h"
#include "MessageClass.h"
#include "MessegesOutMutexClass.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT_CONNECT 27015

using namespace std;

int startClient()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 1;

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    string localIP;

    cout << "Enter IP: ";

    cin >> localIP;

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT_CONNECT);
    inet_pton(AF_INET, localIP.c_str(), &server.sin_addr);

    if (connect(clientSocket, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        std::cout << "Connection failed: " << WSAGetLastError() << '\n';
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server!\n";

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
    WSACleanup();
}
