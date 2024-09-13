#include "file_operating.h"

void createFolder(const std::string& folderName) {
    if (!CreateDirectoryA(folderName.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
        throw std::runtime_error{"Failed to create folder."};
}

void AbstractFilesTraversing::operator() () {
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

void CopyFilesTraversing::fileOperating() {
    if (!CopyFileA((traversingPath + findFileDataA.cFileName).c_str(), (destPath + findFileDataA.cFileName).c_str(), FALSE))
        throw std::runtime_error{"Failed to copy a file"};
    else
        std::cout << "[Copy] Copied to: " << (destPath + findFileDataA.cFileName) << "\r\n";        //debug
}

void DeleteFilesTraversing::fileOperating() {
    if (!DeleteFileA((traversingPath + findFileDataA.cFileName).c_str()))
        throw std::runtime_error{"Failed to delete a file"};
    else 
        std::cout << "[Delete] Deleted: " << (traversingPath + findFileDataA.cFileName) << "\r\n";        //debug
}
