#pragma once
#include <Windows.h>
#include <iostream>
#include <type_traits>
#include <string>
#include <fstream>
#include <vector>
#include "traits.h"
#define PATHSTR_LENGTH 1000

using namespace std::string_literals;

inline std::string getCurrentAbsolutePath();

/* 
1. [x] Create folder
2. [x] Copy files to a folder
3. [ ] Replace data in all files
4. [x] Generate a log for all hashcode of files
5. [ ] Rename all files with a prefix
6. [x] Delete files
 */

inline void createFolder(const std::string& folderName);

// both paths shall exist
void copyFiles(std::string sourcePath, std::string destPath, std::string wildCardMatching = "*"s);

void deleteFiles(std::string targetPath, std::string wildCardMatching = "*"s);

template <class F, typename = std::enable_if_t<can_take<F, std::string&&, std::size_t>::value>>
void readHashInfo(F&& hashInfoConsumer, std::string targetPath, std::string wildCardMatching = "*"s);

// ==============================
//         Definition
// ==============================

inline std::string getCurrentAbsolutePath() {
    char buffer[PATHSTR_LENGTH];
    GetModuleFileNameA(NULL, buffer, PATHSTR_LENGTH);
    std::string strBuffer(buffer);
    std::string::size_type pos = strBuffer.find_last_of("\\/");
    return strBuffer.substr(0, pos + 1);
}

inline void createFolder(const std::string& folderName) {
    if (!CreateDirectoryA(folderName.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
        throw std::runtime_error{"Failed to create folder."};
}

template <class F, typename>
void readHashInfo(F&& hashInfoConsumer, std::string targetPath, std::string wildCardMatching) {
    if (targetPath.back() != '\\' && targetPath.back() != '/')
        targetPath += '/';
    WIN32_FIND_DATAA findFileDataA;
    HANDLE hFind = FindFirstFileA((targetPath + wildCardMatching).c_str(), &findFileDataA);
    if (hFind == INVALID_HANDLE_VALUE) {
        throw std::runtime_error{"Fail to find any file at "s + targetPath + "; with Windows error code "s + std::to_string(GetLastError())};
        return;
    }
    std::string fileNameWithAbsPath;
    do {
        if (!(findFileDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            fileNameWithAbsPath = targetPath + findFileDataA.cFileName;
            if (std::ifstream ifs{fileNameWithAbsPath, std::ios::binary | std::ios::ate}) {
                size_t size = ifs.tellg();
                ifs.seekg(0);
                std::string buffer(size, '\0');
                ifs.read(&buffer[0], size);
                hashInfoConsumer(std::move(fileNameWithAbsPath), std::hash<std::string>{}(buffer));
            } else {
                throw std::runtime_error{"std::ifstream failed to load a file"};
            }
        }
    } while (FindNextFileA(hFind, &findFileDataA));
    FindClose(hFind);
}
