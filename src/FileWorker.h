#pragma once
#include <string>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <commdlg.h>
#include <filesystem>

#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "comdlg32.lib")

class FileWorker
{
public:
    static std::string selectFile();
    static std::string getFileName(const std::string& path);
};

