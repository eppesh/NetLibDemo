// Name: epoll.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/26/2022
// Description:Epoll��

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

    void AddFd(int fd, uint32_t events);              // ���¼�����ע��/��ӹ��ĵ�fd���¼�
    void UpdateChannel(Channel *channel);             // ���¼�����add,modify���ĵ�Channel(��fd)
    std::vector<Channel *> Poll(int timeout = -1);    // ��ѯ�����ؾ�����Channel

private:
    int epoll_fd_;                  // epoll�����fd������Ψһ��ʶ�ں��е�epoll�¼���(�����)
    epoll_event *active_events_;    // ����epoll_wait()�Ĳ���,epoll_wait()����Ѿ������¼�д�뵽�ò����з��ظ��û�;
                                    // �˴��õ���epoll_event�����ָ��,Ҳ������std::vector<epoll_event> active_events_;
};

#endif    // NOLINT