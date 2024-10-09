#include "file_operating.h"

void createFolder(const std::string& folderName) {
    if (!CreateDirectoryA(folderName.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)      // 如果创建文件夹失败，且不是因为尝试创建已有文件夹，则抛出异常。
        throw std::runtime_error{"Failed to create folder."};
}

void AbstractFilesTraversing::operator() () {       // 基本遍历根目录下文件的实现。
    HANDLE hfind = FindFirstFileA((traversingPath + wildcardMatching).c_str(), &findFileDataA);     // FindFirstFileA：Windows API函数，找到根目录下第一个文件。第一个参数是绝对路径；第二个参数是用于存储当前遍历到的文件的信息；返回一个handle，用于遍历。
    if (hfind == INVALID_HANDLE_VALUE) {        // 如果handle是非法的，抛异常。
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
        if (!(findFileDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))       // 遍历到的东西不是文件夹，则执行fileOperating；是文件夹，则执行dirOperating
            fileOperating();
        else 
            dirOperating();
    } while (FindNextFileA(hfind, &findFileDataA));     // 只要还有文件没遍历到，FindNextFileA返回就是真，重复操作
    FindClose(hfind);       // 遍历结束，关闭handle
}

void CopyFilesTraversing::fileOperating() {
    if (!CopyFileA((traversingPath + findFileDataA.cFileName).c_str(), (destPath + findFileDataA.cFileName).c_str(), FALSE))        // 复制文件从A到B，失败则抛出异常。
        throw std::runtime_error{"Failed to copy a file"};
    else
        std::cout << "[Copy] Copied to: " << (destPath + findFileDataA.cFileName) << "\r\n";        //debug
}

void DeleteFilesTraversing::fileOperating() {
    if (!DeleteFileA((traversingPath + findFileDataA.cFileName).c_str()))       // 删除指定文件，失败则抛出异常。
        throw std::runtime_error{"Failed to delete a file"};
    else 
        std::cout << "[Delete] Deleted: " << (traversingPath + findFileDataA.cFileName) << "\r\n";        //debug
}
