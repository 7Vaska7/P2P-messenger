#include "FileWorker.h"

std::string FileWorker::selectFile()
{
    wchar_t fileName[MAX_PATH] = L"";

    OPENFILENAMEW ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;

    ofn.lpstrFilter = L"Все файлы (*.*)\0";
    ofn.nFilterIndex = 1;

    ofn.lpstrTitle = L"Выберите файл";

    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    GetOpenFileNameW(&ofn);

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, fileName, -1, NULL, 0, NULL, NULL);
    std::string strTo(sizeNeeded - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, fileName, -1, &strTo[0], sizeNeeded, NULL, NULL);

    return strTo;
}

std::string FileWorker::getFileName(const std::string& path)
{
    std::filesystem::path p(path);

    std::string fileNameToSent = p.filename().string();

    return fileNameToSent;
}