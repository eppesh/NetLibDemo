// Name: event_loop.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/29/2022
// Description:EventLoop��;
// �¼�ѭ��������Ҫ�������¼���ӵ����¼�ѭ���ڣ�ÿ�����¼�������֪ͨ�������ݲ�ͬfd����ͬ�¼����ͽ��лص�����

#ifndef EVENT_LOOP_H_    // NOLINT
#define EVENT_LOOP_H_    // NOLINT

#include "channel.h"
#include "epoll.h"
#include "thread_pool.h"
#include <sys/epoll.h>

#include <vector>

class Channel;
class Epoll;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    DISALLOW_COPY_AND_MOVE(EventLoop);

    void Loop();
    void UpdateChannel(Channel *channel);

private:
    Epoll *epoll_fd_;
    bool is_quit_;
};

#endif    // NOLINT