// Name: channel.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/26/2022
// Description:Channel类;
// 把fd封装成Channel类;一个fd对应一个Channel类(之前是将fd添加到epoll的事件表/红黑树中,现在是把Channel添加进去)；该类中包含该fd关心的事件以及事件处理函数

#ifndef CHANNEL_H_    // NOLINT
#define CHANNEL_H_    // NOLINT

#include "epoll.h"
#include "event_loop.h"
#include "macros.h"
#include <sys/epoll.h>

#include <functional>

class Epoll;
class EventLoop;

class Channel
{
public:
    // Channel(Epoll *epfd, int fd);
    Channel(EventLoop *event_loop, int fd);
    ~Channel();

    DISALLOW_COPY_AND_MOVE(Channel);

    void EnableReadEvents();    // 关注可读事件
    void HandleEvents();

    int GetFd();
    uint32_t GetEvents();
    uint32_t GetRevents();
    void SetRevents(uint32_t revents);
    bool GetInEpoll();
    void SetInEpoll();
    void SetReadCallback(std::function<void()> read_callback);

    void UseET();    // 使用ET模式

private:
    EventLoop *event_loop_;
    int fd_;              // 该socket对应的fd
    uint32_t events_;     // 关注的事件
    uint32_t revents_;    // 返回的活跃事件;
                          // 当有事件发生时，发生的事件类型会写到revents_中,然后就可以根据这个值来决定执行哪个处理函数
    bool
        in_epoll_;    // 当前Channel是否已经在epoll的事件表（红黑树）中;(可理解为该Channel一一对应的fd是否已经在epoll的事件表中)
    std::function<void()> read_callback_;     // 可读回调函数
    std::function<void()> write_callback_;    // 可写回调函数
};

#endif    // NOLINT