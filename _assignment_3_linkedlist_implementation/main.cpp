#include <iostream>
#include <iomanip>
#include <functional>
#include "file_operating.h"
#include "thread_pool.h"

#include "linkedlist/LinkList.h"


using namespace std::chrono_literals;
using namespace std::string_literals;

std::mutex coutMutex;

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

int main() {
    ThreadPool tp(3);
    LinkList<std::function<void()>> linkedTasksCompound1;
    linkedTasksCompound1.Append(std::bind(createFolder, getCurrentAbsolutePath() + "/file_testing_fields/folder_psd"s));
    linkedTasksCompound1.Append(std::bind(copyFiles, getCurrentAbsolutePath() + "/file_testing_fields/"s, getCurrentAbsolutePath() + "/file_testing_fields/folder_psd"s, "*.psd"s));
    linkedTasksCompound1.Append([]{std::cout << "move psd finifhed." << "\r\n";});
    LinkList<std::function<void()>> linkedTasksCompound2;
    linkedTasksCompound2.Append(std::bind(createFolder, getCurrentAbsolutePath() + "/file_testing_fields/folder_zip"s));
    linkedTasksCompound2.Append(std::bind(copyFiles, getCurrentAbsolutePath() + "/file_testing_fields/"s, getCurrentAbsolutePath() + "/file_testing_fields/folder_zip"s, "*.zip"s));
    linkedTasksCompound2.Append([]{std::cout << "move zip finifhed." << "\r\n";});
    LinkList<std::function<void()>> linkedTasksCompound3;
    linkedTasksCompound3.Append(std::bind(createFolder, getCurrentAbsolutePath() + "/file_testing_fields/folder_html"s));
    linkedTasksCompound3.Append(std::bind(copyFiles, getCurrentAbsolutePath() + "/file_testing_fields/"s, getCurrentAbsolutePath() + "/file_testing_fields/folder_html"s, "*.html"s));
    linkedTasksCompound3.Append([]{std::cout << "move html finifhed." << "\r\n";});

    tp.addTasksCompound(linkedTasksCompound1);
    tp.addTasksCompound(linkedTasksCompound2);
    tp.addTasksCompound(linkedTasksCompound3);

    return 0;
}