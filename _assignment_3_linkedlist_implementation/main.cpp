#include "linkedlist/LinkList.h"
#include "thread_pool.h"
#include <iostream>
#include <iomanip>

using namespace std::chrono_literals;

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
    ThreadPool tp(2);
    std::cout << tp.getThreadCounts() << "\r\n";
    LinkList<std::function<void()>> linkedTasksCompound1{};
    linkedTasksCompound1.Append([]{
        std::this_thread::sleep_for(2000ms);
        {
            std::lock_guard<std::mutex> coutLock{coutMutex};
            std::cout << "Hello there!" << "\r\n";
        }
    });
    linkedTasksCompound1.Append([]{
        std::this_thread::sleep_for(2000ms);
        {
            std::lock_guard<std::mutex> coutLock{coutMutex};
            std::cout << "Have you had your launch yet?" << "\r\n";
        }
    });
    linkedTasksCompound1.Append([]{
        std::this_thread::sleep_for(2000ms);
        {
            std::lock_guard<std::mutex> coutLock{coutMutex};
            std::cout << "What about hang out for a launch";
        }
    });
    linkedTasksCompound1.Append([]{
        std::this_thread::sleep_for(1000ms);
        {
            std::lock_guard<std::mutex> coutLock{coutMutex};
            std::cout << ", with me?" << "\r\n";
        }
    });

    LinkList<std::function<void()>> linkedTasksCompound2{};
    linkedTasksCompound2.Append([]{
        std::this_thread::sleep_for(3000ms);
        {
            std::lock_guard<std::mutex> coutLock{coutMutex};
            std::cout << std::setw(50) << std::right << "Yeah?" << "\r\n";
            std::cout.unsetf(std::ios::right);
        }
    });
    linkedTasksCompound2.Append([]{
        std::this_thread::sleep_for(2000ms);
        {
            std::lock_guard<std::mutex> coutLock{coutMutex};
            std::cout << std::setw(50) << std::right << "Ummm..." << "\r\n";
            std::cout.unsetf(std::ios::right);
        }
    });
    linkedTasksCompound2.Append([]{
        std::this_thread::sleep_for(3000ms);
        {
            std::lock_guard<std::mutex> coutLock{coutMutex};
            std::cout << std::setw(50 - 14) << std::right << "O..OK";
            std::cout.unsetf(std::ios::right);
        }
    });
    linkedTasksCompound2.Append([]{
        std::this_thread::sleep_for(1000ms);
        {
            std::lock_guard<std::mutex> coutLock{coutMutex};
            std::cout << std::right << ", I guess...??" << "\r\n";
            std::cout.unsetf(std::ios::right);
        }
    });

    
    tp.addTasksCompound(linkedTasksCompound1);
    tp.addTasksCompound(linkedTasksCompound2);

    return 0;
}