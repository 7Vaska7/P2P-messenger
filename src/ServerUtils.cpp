#include "ServerUtils.h"

#define DEFAULT_BUFLEN 512

void ServerUtils::messegeGetter(SOCKET& clientSocket, MessegesMutex* mmIn)
{
    int iResult;

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    while (true)
    {
        iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
        {
            string incoming(recvbuf, iResult);

            //mmIn->push("\r\x1b[2K");
            mmIn->push(incoming);

            if (incoming[0] == '\x1A')
            {
                mmIn->push("\n");
                break;
            }
        }
        else if (iResult == 0)
        {
            mmIn->push("\x1A\n\nConnection closed\n");
            break;
        }
        else
        {
            mmIn->push("\x1A\n\nrecv failed\n");
            break;
        }
    }
}

int ServerUtils::waitReturnIP(int port)
{
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    sockaddr_in sender{};
    int senderLen = sizeof(sender);

    SOCKET socketFull = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    BOOL broadcast = TRUE;
    setsockopt(socketFull, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(socketFull,
        (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
        closesocket(socketFull);
        WSACleanup();
        return 1;
    }

    while (true)
    {
        int iResult = recvfrom(
            socketFull,
            recvbuf,
            DEFAULT_BUFLEN - 1,
            0,
            reinterpret_cast<sockaddr*>(&sender),
            &senderLen
        );

        if (iResult == SOCKET_ERROR)
        {
            wprintf(L"recvfrom failed: %ld\n", WSAGetLastError());
            break;
        }

        recvbuf[iResult] = '\0';

        if (strcmp(recvbuf, "DISCOVER") == 0)
        {
            const char* response = "AVAILABLE";

            int sendResult = sendto(
                socketFull,
                response,
                static_cast<int>(strlen(response)),
                0,
                reinterpret_cast<sockaddr*>(&sender),
                senderLen
            );
            wprintf(L"2122123132\n");

            if (sendResult == SOCKET_ERROR)
            {
                wprintf(L"sendto failed: %ld\n", WSAGetLastError());
            }
        }
    }
}