#include "thread_pool.h"

ThreadPool::ThreadPool(size_t threadCounts): threadCounts{threadCounts} {
    for (int i = 0; i < threadCounts; ++i) {
        threads.emplace_back([this](){
            LinkList<std::function<void()>> tasksCompound;
            for (;;) {      // 线程处理函数的本质：一个不break就不结束的循环。一般的，C++线程跑完任务就似了，所以给它一个循坏避免早似。
                {
                    std::unique_lock<std::mutex> lock(this->tasksCompoundsMutex);       // 排异锁：同一时间只有一个线程能进入并在执行该块。e.g. 不能两个线程同时认领复合任务，否则产生悖论be like
                    this->cv.wait(lock, [this](){
                        return this->stop || this->tasksCompounds.NumNodes() != 0;
                    });     // 继续需要满足以下条件：1.刚执行到该语句或有条件变量唤醒了该线程；2.有新复合任务了（tasksCompounds.NumNodes()!=0）或线程池准备被销毁（stop==true）
                    if (this->stop && this->tasksCompounds.NumNodes() == 0) return;     // 循环的唯一出口：任务池被销毁且不再有额外的复合任务
                    this->tasksCompounds.GoTop();       // 到达复合任务链表的首节点
                    tasksCompound = this->tasksCompounds.DeleteCurNode();       // 提取复合任务链表的首节点：即该线程认领一个复合任务
                }
                tasksCompound.GoTop();      // 将认领到的复合任务先定位至首节点处。
                try {       // 开始处理认领的复合任务，如果在处理某一任务时出现异常，则catch异常，报错并不再处理剩下任务。
                    for (int i = 0; i < tasksCompound.NumNodes(); ++i, tasksCompound.Skip()) {      // 遍历复合任务中的各任务并尝试执行。
                        tasksCompound.CurData()();
                    }
                } catch (const std::exception& e) {
                    std::cerr << "An Exception is caught at thread " << std::this_thread::get_id() << "\r\n"; 
                    std::cerr << "what(): " << e.what() << "\r\n";
                } catch (...) {
                    std::cerr << "An Unknown Exception caught at thread " << std::this_thread::get_id() << "\r\n"; 
                }
            }
        });
    }
}


ThreadPool::~ThreadPool() noexcept {
    stop = true;        // 用于告诉处于或执行到cv.wait的线程，线程池要销毁了。
    cv.notify_all();        // 唤醒所有处于代码块（8-16行）的线程，告诉它线程池丸辣（其实就一个线程在那个代码块中）
    for (auto& _thread: threads)        // 确保所有线程都似了
        _thread.join();
}
