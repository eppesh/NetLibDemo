// 名称：thread_pool_test.cpp
// 版权：仅供学习
// 作者：Sean (eppesh@163.com)
// 环境：VS2019
// 时间：05/06/2022
// 说明：线程池测试程序

#include "../src/include/thread_pool.h"

#include <iostream>
#include <string>

void Print(int a, double b, const char *c, std::string d)
{
    std::cout << a << " " << b << " " << c << " " << d << std::endl;
}

void Test()
{
    std::cout << "Hello test." << std::endl;
}

int main()
{
    ThreadPool *thread_poll        = new ThreadPool();
    std::function<void()> function = std::bind(Print, 1, 3.14, "world", std::string("demo"));
    thread_poll->Add(function);
    function = Test;
    thread_poll->Add(function);

    if (thread_poll != nullptr)
    {
        delete thread_poll;
        thread_poll = nullptr;
    }

    return 0;
}