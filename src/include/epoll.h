// Name: epoll.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/26/2022
// Description:Epoll类

#ifndef EPOLL_H_    // NOLINT
#define EPOLL_H_    // NOLINT

#include "macros.h"

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

#include "channel.h"
#include "util.h"

#include <cstring>
#include <unistd.h>
#include <vector>

const int kMaxEvents = 1000;

class Channel;

class Epoll
{
public:
    Epoll();
    ~Epoll();

    DISALLOW_COPY_AND_MOVE(Epoll);

    void AddFd(int fd, uint32_t events);              // 向事件表中注册/添加关心的fd和事件
    void UpdateChannel(Channel *channel);             // 向事件表中add,modify关心的Channel(即fd)
    std::vector<Channel *> Poll(int timeout = -1);    // 轮询并返回就绪的Channel

private:
    int epoll_fd_;                  // epoll自身的fd，用来唯一标识内核中的epoll事件表(红黑树)
    epoll_event *active_events_;    // 传给epoll_wait()的参数,epoll_wait()将会把就绪的事件写入到该参数中返回给用户;
                                    // 此处用的是epoll_event数组的指针,也可以用std::vector<epoll_event> active_events_;
};

#endif    // NOLINT