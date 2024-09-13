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

void createFolder(const std::string& folderName);

#define copyFiles(sourcePath, destPath, ...) CopyFilesTraversing(sourcePath, destPath, ##__VA_ARGS__)()

#define deleteFiles(traversingPath, ...) DeleteFilesTraversing(traversingPath, ##__VA_ARGS__)()

#define writeHashInfo(hashInfoConsumer, traversingPath, ...) WriteHashInfoFilesTraversing(hashInfoConsumer, traversingPath, ##__VA_ARGS__)()

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
    {
        toPath(this->traversingPath);
    }
    void operator() ();
    virtual ~AbstractFilesTraversing() noexcept = default;
};

struct CopyFilesTraversing: public AbstractFilesTraversing{
private:
    std::string destPath;
    void fileOperating() override;
public:
    template<class T1, class T2, class T3 = decltype("*"s), typename = typename std::enable_if<all_convertible<std::string, T1, T2, T3>::value>>
    CopyFilesTraversing(T1&& sourcePath, T2&& destPath, T3&& wildCardMatching = "*"s)
        : AbstractFilesTraversing(std::forward<T1>(sourcePath), std::forward<T3>(wildCardMatching))
        , destPath(std::forward<T2>(destPath)) 
    {
        toPath(this->destPath);
    }
};

struct DeleteFilesTraversing: public AbstractFilesTraversing {
private:
    void fileOperating() override;
public:
    template<class T1, class T2 = decltype("*"s), typename = typename std::enable_if<all_convertible<std::string, T1, T2>::value>>
    DeleteFilesTraversing(T1&& traversingPath, T2&& wildCardMatching = "*"s): AbstractFilesTraversing(std::forward<T1>(traversingPath), std::forward<T2>(wildCardMatching)) {}
};

template <class F>
struct WriteHashInfoFilesTraversing: public AbstractFilesTraversing {
private:
    F hashInfoConsumer;
    void fileOperating();
public:
    template <class T1, class T2 = decltype("*"s), typename = typename std::enable_if<all_convertible<std::string, T1, T2>::value && can_take<F, std::string&&, std::size_t>::value>>
    WriteHashInfoFilesTraversing(F&& hashInfoConsumer, T1&& traversingPath, T2&& wildCardMatching = "*"s)
        : AbstractFilesTraversing(std::forward<T1>(traversingPath), std::forward<T2>(wildCardMatching)) 
        , hashInfoConsumer(std::forward<F>(hashInfoConsumer))
    {}
};

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

template <class F>
void WriteHashInfoFilesTraversing<F>::fileOperating() {
    std::cout << "findFileDataA.cFileName: " << findFileDataA.cFileName << "\r\n";      //debug
    if (std::ifstream ifs{traversingPath + findFileDataA.cFileName, std::ios::binary | std::ios::ate}) {
        size_t size = ifs.tellg();
        ifs.seekg(0);
        std::string buffer(size, '\0');
        ifs.read(&buffer[0], size);
        hashInfoConsumer(std::move(traversingPath + findFileDataA.cFileName), std::hash<std::string>{}(buffer));
    } else {
        throw std::runtime_error{"std::ifstream failed to load a file"};
    }
}
