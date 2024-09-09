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

inline void toPath(std::string& dirOrPath);


/* 
1. [x] Create folder
2. [x] Copy files to a folder
3. [ ] Replace data in all files
4. [x] Generate a log for all hashcode of files
5. [ ] Rename all files with a prefix
6. [x] Delete files
 */

struct AbstractFilesTraversing {
private:
protected:
    std::string traversingPath;
    std::string wildcardMatching;
    WIN32_FIND_DATAA findFileDataA;
    virtual void fileOperating() = 0;
    virtual void dirOperating() {}
public:
    template<class T1, class T2, typename = typename std::enable_if<all_convertible<std::string, T1, T2>::value>>
    AbstractFilesTraversing(T1&& traversingPath, T2&& wildcardMatching)
        : traversingPath{std::forward<T1>(traversingPath)}
        , wildcardMatching{std::forward<T2>(wildcardMatching)} 
    {}
    void operator() () {
        toPath(traversingPath);
        HANDLE hfind = FindFirstFileA((traversingPath + wildcardMatching).c_str(), &findFileDataA);
        if (hfind == INVALID_HANDLE_VALUE) {
            switch (GetLastError()) {
                case 0x2: 
                    throw std::runtime_error{"No file is found in the directory with wildcard matching case: "s + traversingPath + wildcardMatching + "; with Windows error code "s + std::to_string(GetLastError())};
                    break;
                case 0x3:
                    throw std::runtime_error{"The path with wildcard mathcing case is invalid: "s + traversingPath + wildcardMatching + "; with Windows error code "s + std::to_string(GetLastError())};
                    break;
                default: 
                    throw std::runtime_error{"Fail to find any file at "s + traversingPath + wildcardMatching + "; with Windows error code "s + std::to_string(GetLastError())};
            }
        }
        do {
            if (!(findFileDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                fileOperating();
            else 
                dirOperating();
        } while (FindNextFileA(hfind, &findFileDataA));
        FindClose(hfind);
    }
    virtual ~AbstractFilesTraversing() noexcept = default;
};

struct CopyFilesTraversing: public AbstractFilesTraversing{
private:
    std::string destPath;
public:
    template<class T1, class T2, class T3, typename = typename std::enable_if<all_convertible<std::string, T1, T2, T3>::value>>
    CopyFilesTraversing(T1&& sourcePath, T2&& destPath, T3&& wildCardMatching = "*"s)
        : AbstractFilesTraversing(std::forward<T1>(sourcePath), std::forward<T3>(wildCardMatching))
        , destPath(std::forward<T2>(destPath)) 
    {
        toPath(this->destPath);
    }
    void fileOperating() override {
        std::cout << "helloworld from CopyFilesTraversing::fileOperating" << "\r\n";        //debug
        if (!CopyFileA((traversingPath + findFileDataA.cFileName).c_str(), (destPath + findFileDataA.cFileName).c_str(), FALSE))
            throw std::runtime_error{"Failed to copy a file"};
        else
            std::cout << "[Copy] Copied to: " << (destPath + findFileDataA.cFileName) << "\r\n";        //debug
    }
};

struct DeleteFilesTraversing: public AbstractFilesTraversing {
    template<class T1, class T2, typename = typename std::enable_if<all_convertible<std::string, T1, T2>::value>>
    DeleteFilesTraversing(T1&& traversingPath, T2&& wildCardMatching = "*"s): AbstractFilesTraversing(std::forward<T1>(traversingPath), std::forward<T2>(wildCardMatching)) {}
    void fileOperating() override {
        std::cout << "helloworld from DeleteFilesTraversing::fileOperating" << "\r\n";        //debug
        if (!DeleteFileA((traversingPath + findFileDataA.cFileName).c_str()))
            throw std::runtime_error{"Failed to delete a file"};
        else 
            std::cout << "[Delete] Deleted: " << (traversingPath + findFileDataA.cFileName) << "\r\n";        //debug
    }
};

struct WriteHashInfoFilesTraversing: public AbstractFilesTraversing {
    template<class T1, class T2, typename = typename std::enable_if<all_convertible<std::string, T1, T2>::value>>
    WriteHashInfoFilesTraversing(T1&& traversingPath, T2&& wildCardMatching = "*"s): AbstractFilesTraversing(std::forward<T1>(traversingPath), std::forward<T2>(wildCardMatching)) {}
    void fileOperating() override {
        std::cout << "helloworld from DeleteFilesTraversing::fileOperating" << "\r\n";        //debug
        if (!DeleteFileA((traversingPath + findFileDataA.cFileName).c_str()))
            throw std::runtime_error{"Failed to delete a file"};
        else 
            std::cout << "[Delete] Deleted: " << (traversingPath + findFileDataA.cFileName) << "\r\n";        //debug
    }
};

void createFolder(const std::string& folderName);

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

inline void toPath(std::string& dirOrPath) {
    if (dirOrPath.back() != '\\' && dirOrPath.back() != '/')
        dirOrPath = std::move(dirOrPath) + '/';
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
