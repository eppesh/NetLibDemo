#include "include/thread_pool.h"

ThreadPool::ThreadPool(unsigned int size /* = std::thread::hardware_concurrency() */) : stop_(false)
{
    // 创建size个新线程塞进线程池
    for (unsigned int i = 0; i < size; ++i)
    {
        threads_.emplace_back(std::thread([this]() {
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mutex_);
                    cv_.wait(lock, [this]() { return (stop_ || !tasks_queue_.empty()); });
                    if (stop_ && tasks_queue_.empty())
                    {
                        return;
                    }
                    task = tasks_queue_.front();
                    tasks_queue_.pop();
                }
                task();
            }
        }));
    }    // for
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for (std::thread &thread : threads_)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}
