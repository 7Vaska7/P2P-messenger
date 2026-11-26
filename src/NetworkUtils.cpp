#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

std::string getLocalIp()
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        std::cerr << "gethostname failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        char youAreinvalid[4] = "-1";
        return youAreinvalid;
    }

    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* result = nullptr;
    if (getaddrinfo(hostname, nullptr, &hints, &result)) {
        std::cerr << "gethostbyname failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        char youAreinvalid[4] = "-1";
        return youAreinvalid;
    }

    addrinfo* ptr = result->ai_next;

    char ipStr[INET6_ADDRSTRLEN];

    for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next)
    {
        if (ptr->ai_family == AF_INET)
        {
            auto* ipv4 = reinterpret_cast<sockaddr_in*>(ptr->ai_addr);

            inet_ntop(AF_INET, &ipv4->sin_addr, ipStr, sizeof(ipStr));
            break;
        }
    }

    freeaddrinfo(result);

    if (ipStr[0] == '\0') {
        return "-1";
    }

    return ipStr;
}