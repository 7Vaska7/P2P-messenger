#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>
#include <conio.h>
#include "MessageClass.h"
#include "MessegesOutMutexClass.h"
#include "FileWorker.h"

#define DEFAULT_BUFLEN 512

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "comdlg32.lib")

using namespace std;

string getLocalIp()
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

void messegesGetter(SOCKET& clientSocket, Messege& messege, MessegesMutex* mmOut)
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

            mmOut->push("\r\x1b[2K");
            mmOut->push(incoming);

            if (incoming[0] == '\x1A')
            {
                mmOut->push("\n");
                break;
            }
            mmOut->push("\n>> ");
            mmOut->push(messege.getMessege());
        }
        else if (iResult == 0)
        {
            mmOut->push("\x1A\n\nConnection closed\n");
            break;
        }
        else
        {
            mmOut->push("\x1A\n\nrecv failed\n");
            break;
        }
    }
}

void messegesReader(SOCKET& clientSocket, Messege& messege, MessegesMutex* mmOut)
{
    bool canWrite = true;

    while (true)
    {
        if (_kbhit() && canWrite)
        {
            unsigned char ch = _getch();

            if (ch == '\r' || ch == '\n')
            {
                mmOut->push("\r\x1b[2K");
                mmOut->push(messege.getMessege());
                mmOut->push("\n>> ");

                int sent = send(
                    clientSocket,
                    messege.c_str(),
                    (int)messege.size(),
                    0
                );
                messege.deleteMessege();

                if (sent == SOCKET_ERROR)
                {
                    break;
                }
            }
            else if (ch == '\b')
            {
                if (!messege.isEmpty())
                {
                    messege.backspaceMessege();
                    mmOut->push("\b \b");
                }
            }
            else if (ch == 224 || ch == 0)
            {
                ch = _getch();

                if (ch == 75)
                {
                    messege.arrowsFunc(ARROWLEFT);
                    mmOut->push("\x1b[D");
                }
                else if (ch == 77)
                {
                    messege.arrowsFunc(ARROWRIGHT);
                    mmOut->push("\x1b[C");
                }

                else if (ch == 134)
                {
                    mmOut->push("\r\x1b[2K");
                    mmOut->push("You have ended the conversation");
                    mmOut->push("\n");

                    const char* exitMsg = "\x1AThe person you were speaking with has ended the conversation";
                    int sent = send(
                        clientSocket,
                        exitMsg,
                        (int)strlen(exitMsg),
                        0
                    );
                    break;
                }

                else if (ch == 60)
                {
                    canWrite = false;

                    messege.deleteMessege();

                    string filePath = FileWorker::selectFile();
                    
                    string toSent = "FILE: " + FileWorker::getFileName(filePath) + "\nFILE:\t f5 to Load";

                    mmOut->push("\r\x1b[2K");
                    mmOut->push(toSent);
                    mmOut->push("\n>> ");

                    int sent = send(
                        clientSocket,
                        toSent.c_str(),
                        (int)strlen(toSent.c_str()),
                        0
                    );

                    canWrite = true;
                }
            }
            else
            {
                messege.addSybmol(ch);

                string s(1, ch);
                mmOut->push(s);
            }
        }
    }
}