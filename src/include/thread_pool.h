// Name: thread_pool.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 05/06/2022
// Description:ThreadPool��, �̳߳�

#ifndef THREAD_POOL_H_    // NOLINT
#define THREAD_POOL_H_    // NOLINT

#include "macros.h"
#include <condition_variable>

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    explicit ThreadPool(unsigned int size = std::thread::hardware_concurrency());
    ~ThreadPool();

    DISALLOW_COPY_AND_MOVE(ThreadPool);

    //  ������������������
    template<class F, class... Args>
    auto Add(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> threads_;                 // �̳߳صĹ����߳���
    std::queue<std::function<void()>> tasks_queue_;    // �������;�ö�����Ԫ���Ǿ����¼��󶨵Ļص�����

    // ͬ������: ����������������
    std::mutex tasks_mutex_;
    std::condition_variable cv_;
    bool stop_;    // ����Ƿ�ر��̳߳أ�����ʹ��std::atomic<bool>����
};

// ���ܷ���cpp�ļ���,��ΪC++��������֧��ģ��ķ������
template<class F, class... Args>
auto ThreadPool::Add(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task =
        std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        // Don't allow enqueueing after stopping the pool
        if (stop_)
        {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        tasks_queue_.emplace([task]() { (*task)(); });
    }
    cv_.notify_one();
    return res;
}
#endif    // NOLINT