#include <iostream>
#include <iomanip>
#include <functional>
#include <exception>
#include "file_operating.h"
#include "thread_pool.h"

#include "linkedlist/LinkList.h"


using namespace std::chrono_literals;
using namespace std::string_literals;

std::mutex coutMutex;       // 终端输出的mutex，防止多线程向终端输出时乱了套

int main() {
    LinkList<std::string> linkedFileHashInfo3;      // 用于记录多文件哈希校验码的链表
    LinkList<std::string> linkedFileHashInfo4;      // 用于记录多文件哈希校验码的链表
    ThreadPool tp(4);       // 创建一个有四个线程的线程池

    LinkList<std::function<void()>> linkedTasksCompound1;       // 创建一个复合任务
    linkedTasksCompound1.Append(std::bind(createFolder, getCurrentAbsolutePath() + "file_testing_fields/folder_psd"s));     // 追加任务：创建文件夹"当前所在路径的绝对路径+file_testing_fields/folder_psd"，以下将当前所在路径的绝对路径简称为.
    linkedTasksCompound1.Append(std::bind(&CopyFilesTraversing::operator(), CopyFilesTraversing{getCurrentAbsolutePath() + "file_testing_fields/"s, getCurrentAbsolutePath() + "file_testing_fields/folder_psd"s, "*.psd"s}));      // 追加任务：将扩展名为.psd的文件从"./file_testing_fields/"移动到"./file_testing_fields/folder_psd"
    linkedTasksCompound1.Append(std::bind(&DeleteFilesTraversing::operator(), DeleteFilesTraversing{getCurrentAbsolutePath() + "file_testing_fields/"s, "*.psd"s}));        // 追加任务：删除所有"./file_testing_fields/"路径下扩展名为.psd的文件
    linkedTasksCompound1.Append([]{std::cout << "move psd finifhed." << "\r\n";});      // 追加任务：向终端输出处理完毕信息


    LinkList<std::function<void()>> linkedTasksCompound2;
    linkedTasksCompound2.Append(std::bind(createFolder, getCurrentAbsolutePath() + "file_testing_fields/folder_zip"s));     // 同linkedTasksCompound1，解释见【linkedTasksCompound1】
    linkedTasksCompound2.Append(std::bind(&CopyFilesTraversing::operator(), CopyFilesTraversing{getCurrentAbsolutePath() + "file_testing_fields/"s, getCurrentAbsolutePath() + "file_testing_fields/folder_zip"s, "*.zip"s}));
    linkedTasksCompound2.Append(std::bind(&DeleteFilesTraversing::operator(), DeleteFilesTraversing{getCurrentAbsolutePath() + "file_testing_fields/"s, "*.zip"s}));
    linkedTasksCompound2.Append([]{std::cout << "move zip finifhed." << "\r\n";});


    LinkList<std::function<void()>> linkedTasksCompound3;
    linkedTasksCompound3.Append([&linkedFileHashInfo3]{     // 追加任务：将文件名及文件哈希校验码以字符串形式存入链表linkedFileHashInfo3
        writeHashInfo([&linkedFileHashInfo3](std::string&& _fileLoc, std::size_t _hashcode){
            linkedFileHashInfo3.Append(std::move(_fileLoc) + "\t\t\t\t\t"s + std::to_string(_hashcode));
        }, getCurrentAbsolutePath() + "file_testing_fields/"s, "*.html"s);
    });
    linkedTasksCompound3.Append([&linkedFileHashInfo3]{     // 追加任务：将链表linkedFileHashInfo3的内容保存到"./file_testing_fields/html-file-hash.log"
        linkedFileHashInfo3.Save(".\\file_testing_fields/html-file-hash.log");
    });
    linkedTasksCompound3.Append(std::bind(createFolder, getCurrentAbsolutePath() + "file_testing_fields/folder_html"s));        // 此后同linkedTasksCompound1，解释见【linkedTasksCompound1】
    linkedTasksCompound3.Append(std::bind(&CopyFilesTraversing::operator(), CopyFilesTraversing{getCurrentAbsolutePath() + "file_testing_fields/"s, getCurrentAbsolutePath() + "file_testing_fields/folder_html"s, "*.html"s}));
    linkedTasksCompound3.Append(std::bind(&DeleteFilesTraversing::operator(), DeleteFilesTraversing{getCurrentAbsolutePath() + "file_testing_fields/"s, "*.html"s}));
    linkedTasksCompound3.Append([]{std::cout << "move html finifhed." << "\r\n";});


    LinkList<std::function<void()>> linkedTasksCompound4;
    linkedTasksCompound4.Append([&linkedFileHashInfo4]{     // 同linkedTasksCompound3，解释见【linkedTasksCompound3】
        writeHashInfo([&linkedFileHashInfo4](std::string&& _fileLoc, std::size_t _hashcode){
            linkedFileHashInfo4.Append(std::move(_fileLoc) + "\t\t\t\t\t"s + std::to_string(_hashcode));
        }, getCurrentAbsolutePath() + "file_testing_fields/"s, "*.jpg"s);
    });
    linkedTasksCompound4.Append([&linkedFileHashInfo4]{
        linkedFileHashInfo4.Save(".\\file_testing_fields/jpg-file-hash.log");
    });
    linkedTasksCompound4.Append(std::bind(createFolder, getCurrentAbsolutePath() + "file_testing_fields/folder_jpg"s));
    linkedTasksCompound4.Append(std::bind(&CopyFilesTraversing::operator(), CopyFilesTraversing{getCurrentAbsolutePath() + "file_testing_fields/"s, getCurrentAbsolutePath() + "file_testing_fields/folder_jpg"s, "*.jpg"s}));
    linkedTasksCompound4.Append(std::bind(&DeleteFilesTraversing::operator(), DeleteFilesTraversing{getCurrentAbsolutePath() + "file_testing_fields/"s, "*.jpg"s}));
    linkedTasksCompound4.Append([]{std::cout << "move jpg finifhed." << "\r\n";});



    tp.addTasksCompound(linkedTasksCompound1);      // 添加复合任务到线程池中去处理。
    tp.addTasksCompound(linkedTasksCompound2);      // 添加复合任务到线程池中去处理。
    tp.addTasksCompound(linkedTasksCompound3);      // 添加复合任务到线程池中去处理。
    tp.addTasksCompound(linkedTasksCompound4);      // 添加复合任务到线程池中去处理。
    

    return 0;
}

void archivedTest1() {
    int arr[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, -1, -2, -3, -4, -5};
    LinkList linkedList{sizeof(arr) / sizeof(std::remove_pointer_t<std::decay_t<decltype(arr)>>), arr};
    std::cout << linkedList << "\r\n";

    linkedList.Reverse();
    linkedList.PutList(std::cout);
    std::cout << "\r\n";

    linkedList.Save("-.log");

    std::cout << linkedList.CurPos() << "\r\n";
    linkedList.GoTop();
    std::cout << linkedList.CurPos() << "\r\n";
    linkedList.Sort(int(), true);
    std::cout << linkedList << "\r\n";
}

void archivedTest2() {
    try {
        createFolder(getCurrentAbsolutePath() + "file_testing_fields/test"s);
        copyFiles((getCurrentAbsolutePath() + "file_testing_fields/"s).c_str(), getCurrentAbsolutePath() + "file_testing_fields/test"s);
        LinkList<std::string> linkedFileHashInfo;
        writeHashInfo([&linkedFileHashInfo](std::string&& _fileLoc, std::size_t _hashcode){
            linkedFileHashInfo.Append(std::move(_fileLoc) + "\t\t\t\t\t"s + std::to_string(_hashcode));
        }, getCurrentAbsolutePath() + "file_testing_fields/test"s, "*"s);
        linkedFileHashInfo.Save(".\\file_testing_fields\\log.log");
        deleteFiles(getCurrentAbsolutePath() + "file_testing_fields/test"s, "*"s);
    } catch (std::exception& e) {
        std::cout << e.what() << "\r\n";
    }

}