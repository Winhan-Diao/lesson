#include "file_operating.h"

// both paths shall exist
void copyFiles(std::string sourcePath, std::string destPath, std::string wildCardMatching) {
    if (sourcePath.back() != '\\' && sourcePath.back() != '/')
        sourcePath += '/';
    if (destPath.back() != '\\' && destPath.back() != '/')
        destPath += '/';
    std::string sourcePathMatchingFiles = sourcePath + wildCardMatching;
    std::cout << sourcePathMatchingFiles << "\r\n";     //debug
    WIN32_FIND_DATAA findFileDataA;
    HANDLE hFind = FindFirstFileA(sourcePathMatchingFiles.c_str(), &findFileDataA);
    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Fail to find any file at " << sourcePath << "; with error code " << GetLastError() << "\r\n";
        return;
    }
    do {
        if (!(findFileDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            std::cout << (sourcePath + findFileDataA.cFileName) << "\r\n";      //debug
            std::cout << (destPath + findFileDataA.cFileName) << "\r\n";        //debug
            if (!CopyFileA((sourcePath + findFileDataA.cFileName).c_str(), (destPath + findFileDataA.cFileName).c_str(), FALSE))
                std::cerr << "Failed to copy a file" << "\r\n";
        }
    } while (FindNextFileA(hFind, &findFileDataA));
    FindClose(hFind);
}

void deleteFiles(std::string targetPath, std::string wildCardMatching) {
    if (targetPath.back() != '\\' && targetPath.back() != '/')
        targetPath += '/';
    WIN32_FIND_DATAA findFileDataA;
    HANDLE hFind = FindFirstFileA((targetPath + wildCardMatching).c_str(), &findFileDataA);
    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Fail to find any file at " << targetPath << "; with error code " << GetLastError() << "\r\n";
        return;
    }
    do {
        if (!(findFileDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (!DeleteFileA((targetPath + findFileDataA.cFileName).c_str()))
                std::cerr << "Failed to delete a file" << "\r\n";
        }
    } while (FindNextFileA(hFind, &findFileDataA));
    FindClose(hFind);
}
