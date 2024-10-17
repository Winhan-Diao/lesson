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

inline std::string getCurrentAbsolutePath();        // 获得执行文件所在目录的绝对路径【定义见下】

inline void toPath(std::string& dirOrPath);     // 确保字符串末尾有一个/，以代表路径【定义见下】


/* 
1. [x] Create folder
2. [x] Copy files to a folder
3. [ ] Replace data in all files
4. [x] Generate a log for all hashcode of files
5. [ ] Rename all files with a prefix
6. [x] Delete files
 */

void createFolder(const std::string& folderName);       // 依据所给的绝对路径+待创建文件夹，创建这个文件夹【定义见file_operating.cpp】

#define copyFiles(sourcePath, destPath, ...) CopyFilesTraversing(sourcePath, destPath, ##__VA_ARGS__)()     // CopyFilesTraversing的助手宏

#define deleteFiles(traversingPath, ...) DeleteFilesTraversing(traversingPath, ##__VA_ARGS__)()     // DeleteFilesTraversing的助手宏

#define writeHashInfo(hashInfoConsumer, traversingPath, ...) WriteHashInfoFilesTraversing(hashInfoConsumer, traversingPath, ##__VA_ARGS__)()        // WriteHashInfoFilesTraversing的助手宏

struct AbstractFilesTraversing {        // 抽象文件遍历类：筛选所有根目录下符合扩展名通配符的文件并遍历
private:
protected:
    std::string traversingPath;
    std::string wildcardMatching;
    WIN32_FIND_DATAA findFileDataA;
    virtual void fileOperating() = 0;       // 对根目录下文件的操作，必须override
    virtual void dirOperating() {}      // 对根目录下文件夹的操作，可选override，在该例中未使用。
public:
    template<class T1, class T2, typename = typename std::enable_if<all_convertible<std::string, T1, T2>::value>>
    AbstractFilesTraversing(T1&& traversingPath, T2&& wildcardMatching)
        : traversingPath{std::forward<T1>(traversingPath)}
        , wildcardMatching{std::forward<T2>(wildcardMatching)} 
    {
        toPath(this->traversingPath);
    }
    void operator() ();     // 实现基本的遍历操作【定义见file_operating.cpp】
    virtual ~AbstractFilesTraversing() noexcept = default;
};

struct CopyFilesTraversing: public AbstractFilesTraversing{     // 复制所有根目录下符合扩展名通配符的文件至指定目录下。
private:
    std::string destPath;
    void fileOperating() override;      // 抽象文件操作函数的“复制”实现【定义见file_operating.cpp】
public:
    template<class T1, class T2, class T3 = decltype("*"s), typename = typename std::enable_if<all_convertible<std::string, T1, T2, T3>::value>>
    CopyFilesTraversing(T1&& sourcePath, T2&& destPath, T3&& wildCardMatching = "*"s)
        : AbstractFilesTraversing(std::forward<T1>(sourcePath), std::forward<T3>(wildCardMatching))
        , destPath(std::forward<T2>(destPath)) 
    {
        toPath(this->destPath);
    }
};

struct DeleteFilesTraversing: public AbstractFilesTraversing {      // 删除所有根目录下符合扩展名通配符的文件
private:
    void fileOperating() override;      // 抽象文件操作函数的“删除”实现【定义见file_operating.cpp】
public:
    template<class T1, class T2 = decltype("*"s), typename = typename std::enable_if<all_convertible<std::string, T1, T2>::value>>
    DeleteFilesTraversing(T1&& traversingPath, T2&& wildCardMatching = "*"s): AbstractFilesTraversing(std::forward<T1>(traversingPath), std::forward<T2>(wildCardMatching)) {}
};

template <class F>
struct WriteHashInfoFilesTraversing: public AbstractFilesTraversing {       // 记录所有根目录下符合扩展名通配符的文件的哈希校验码到xx-file-hash.log中
private:
    F hashInfoConsumer;
    void fileOperating();       // 抽象文件操作函数的“复制”实现【定义见下】
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
    GetModuleFileNameA(NULL, buffer, PATHSTR_LENGTH);       // Windows API函数，用于获得当前执行模块的绝对路径。
    std::string strBuffer(buffer);
    std::string::size_type pos = strBuffer.find_last_of("\\/");
    return strBuffer.substr(0, pos + 1);
}

inline void toPath(std::string& dirOrPath) {
    if (dirOrPath.back() != '\\' && dirOrPath.back() != '/')
        dirOrPath = std::move(dirOrPath) + '/';     // 如果所给字符串末尾没有'/'，就加一个
}

template <class F>
void WriteHashInfoFilesTraversing<F>::fileOperating() {
    std::cout << "[Hash] " << findFileDataA.cFileName << "\r\n";      //debug
    if (std::ifstream ifs{traversingPath + findFileDataA.cFileName, std::ios::binary | std::ios::ate}) {
        size_t size = ifs.tellg();
        ifs.seekg(0);
        std::string buffer(size, '\0');
        ifs.read(&buffer[0], size);     // 将文件内容读入缓冲区（从存储到内存），用于接下来获取哈希校验码
        hashInfoConsumer(std::move(traversingPath + findFileDataA.cFileName), std::hash<std::string>{}(buffer));        // 获取文件哈希校验码，invoke 处理哈希校验码的consumer（创建该类时传入的F）。
    } else {
        throw std::runtime_error{"std::ifstream failed to load a file"};
    }
}
