#pragma once
#include <thread>
#include <functional>
#include <mutex>
#include <vector>
#include <queue>
#include <atomic>
#include <type_traits>
#include<condition_variable>
#include "linkedlist/LinkList.h"

namespace tests {
    static double s, t;
}

class ThreadPool {
private:
    size_t threadCounts;
    std::vector<std::thread> threads{};
    LinkList<LinkList<std::function<void()>>> tasksCompounds{};

    std::mutex tasksCompoundsMutex;
    std::condition_variable cv;
    std::atomic_bool stop{false};
public:
    ThreadPool(size_t threadCounts);

    template <class T, typename = std::enable_if_t<std::is_convertible_v<T, LinkList<std::function<void()>>>>>
    void addTasksCompound(T&& tasksCompund);

    inline size_t getThreadCounts() noexcept;
    inline decltype(tasksCompounds.NumNodes()) getAwaitingTasksCompoundCounts() noexcept;

    ~ThreadPool() noexcept;
};

// ==============================
//         Definition
// ==============================

template <class T, typename>
void ThreadPool::addTasksCompound(T&& tasksCompund) {
    {
        std::unique_lock<std::mutex> tasksCompoundLock{tasksCompoundsMutex};
        tasksCompounds.Append(tasksCompund);
    }
    cv.notify_one();
}

inline size_t ThreadPool::getThreadCounts() noexcept {
    return threadCounts;
}

inline auto ThreadPool::getAwaitingTasksCompoundCounts() noexcept -> decltype(tasksCompounds.NumNodes()) {
    return tasksCompounds.NumNodes();
}
