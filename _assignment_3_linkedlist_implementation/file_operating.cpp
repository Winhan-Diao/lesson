#include "file_operating.h"

// both paths shall exist
void copyFiles(std::string sourcePath, std::string destPath, std::string wildCardMatching) {
    if (sourcePath.back() != '\\' && sourcePath.back() != '/')
        sourcePath += '/';
    if (destPath.back() != '\\' && destPath.back() != '/')
        destPath += '/';
    std::string sourcePathMatchingFiles = sourcePath + wildCardMatching;
    std::cout << "[Copy] Match case: " << sourcePathMatchingFiles << "\r\n";     //debug
    WIN32_FIND_DATAA findFileDataA;
    HANDLE hFind = FindFirstFileA(sourcePathMatchingFiles.c_str(), &findFileDataA);
    if (hFind == INVALID_HANDLE_VALUE) {
        throw std::runtime_error{"Fail to find any file at "s + sourcePath + "; with Windows error code "s + std::to_string(GetLastError())};
        return;
    }
    do {
        if (!(findFileDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (!CopyFileA((sourcePath + findFileDataA.cFileName).c_str(), (destPath + findFileDataA.cFileName).c_str(), FALSE))
                throw std::runtime_error{"Failed to copy a file"};
            else
                std::cout << "[Copy] Copied to: " << (destPath + findFileDataA.cFileName) << "\r\n";        //debug
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
        throw std::runtime_error{"Fail to find any file at "s + targetPath + "; with Windows error code "s + std::to_string(GetLastError())};
        return;
    }
    do {
        if (!(findFileDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (!DeleteFileA((targetPath + findFileDataA.cFileName).c_str()))
                throw std::runtime_error{"Failed to delete a file"};
            else 
                std::cout << "[Delete] Deleted: " << (targetPath + findFileDataA.cFileName) << "\r\n";        //debug
        }
    } while (FindNextFileA(hFind, &findFileDataA));
    FindClose(hFind);
}
