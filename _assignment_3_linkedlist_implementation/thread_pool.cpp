#include "thread_pool.h"

ThreadPool::ThreadPool(size_t threadCounts): threadCounts{threadCounts} {
    for (int i = 0; i < threadCounts; ++i) {
        threads.emplace_back([this](){
            LinkList<std::function<void()>> tasksCompound;
            for (;;) {
                {
                    std::unique_lock<std::mutex> lock(this->tasksCompoundsMutex);
                    this->cv.wait(lock, [this](){
                        return this->stop || this->tasksCompounds.NumNodes() != 0;
                    });
                    if (this->stop && this->tasksCompounds.NumNodes() == 0) return;
                    this->tasksCompounds.GoTop();
                    tasksCompound = this->tasksCompounds.DeleteCurNode();
                }
                tasksCompound.GoTop();
                try {
                    for (int i = 0; i < tasksCompound.NumNodes(); ++i, tasksCompound.Skip()) {
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
    stop = true;
    cv.notify_all();
    for (auto& _thread: threads)
        _thread.join();
    tests::t=clock();
    double a=tests::s,b=tests::t;
    printf("costing time : %.2lf ms",b-a);
}
