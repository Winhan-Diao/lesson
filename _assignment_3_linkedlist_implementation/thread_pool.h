#pragma once
#include <thread>
#include <functional>
#include <mutex>
#include <vector>
#include <queue>
#include <atomic>
#include <type_traits>
#include <condition_variable>
#include "linkedlist/LinkList.h"

class ThreadPool {
private:
    size_t threadCounts;        // 线程池的线程数量
    std::vector<std::thread> threads{};     // 具体的线程对象，存储于标准库的向量类对象中
    LinkList<LinkList<std::function<void()>>> tasksCompounds{};     // 用于存储复合任务的链表，PS复合任务本身也是任务的链表。

    std::mutex tasksCompoundsMutex;     // 复合任务链表的mutex。PS：mutex为mutual exclusive，相当于锁的参数，具有同一个mutex的锁才能有效锁住一块代码。
    std::condition_variable cv;     // 条件变量，
    std::atomic_bool stop{false};       // stop是一个原子布尔变量，使用stop，当stop==true时意味着线程处于销毁状态。
public:
    ThreadPool(size_t threadCounts);        // 线程池构造器，创建指定线程数量的线程池【定义见thread_pool.cpp】

    template <class T, typename = std::enable_if_t<std::is_convertible_v<T, LinkList<std::function<void()>>>>>
    void addTasksCompound(T&& tasksCompund);        // 该函数用于往线程池添加一个复合任务【定义见下】

    inline size_t getThreadCounts() noexcept;       // 该函数用于线程池所持有的获取线程数量【定义见下】
    inline decltype(tasksCompounds.NumNodes()) getAwaitingTasksCompoundCounts() noexcept;       // 该函数用于获取正等待处理的复合任务的数量。【定义见下】

    ~ThreadPool() noexcept;     // 销毁任务池时的自定义操作【定义见thread_pool.cpp】
};

// ==============================
//         Definition
// ==============================

template <class T, typename>
void ThreadPool::addTasksCompound(T&& tasksCompund) {
    {
        std::unique_lock<std::mutex> tasksCompoundLock{tasksCompoundsMutex};        // 锁住代码块，保证只有一个线程在操作复合任务的链表
        tasksCompounds.Append(tasksCompund);        // 往复合任务的链表末尾添加一个复合任务
    }
    cv.notify_one();        // 唤醒处于cv.wait的一个线程：做任务辣
}

inline size_t ThreadPool::getThreadCounts() noexcept {
    return threadCounts;
}

inline auto ThreadPool::getAwaitingTasksCompoundCounts() noexcept -> decltype(tasksCompounds.NumNodes()) {
    return tasksCompounds.NumNodes();
}
