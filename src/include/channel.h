// Name: channel.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/26/2022
// Description:Channel��;
// ��fd��װ��Channel��;һ��fd��Ӧһ��Channel��(֮ǰ�ǽ�fd��ӵ�epoll���¼���/�������,�����ǰ�Channel��ӽ�ȥ)�������а�����fd���ĵ��¼��Լ��¼�������

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

    void EnableReadEvents();    // ��ע�ɶ��¼�
    void HandleEvents();

    int GetFd();
    uint32_t GetEvents();
    uint32_t GetRevents();
    void SetRevents(uint32_t revents);
    bool GetInEpoll();
    void SetInEpoll();
    void SetReadCallback(std::function<void()> read_callback);

    void UseET();    // ʹ��ETģʽ

private:
    EventLoop *event_loop_;
    int fd_;              // ��socket��Ӧ��fd
    uint32_t events_;     // ��ע���¼�
    uint32_t revents_;    // ���صĻ�Ծ�¼�;
                          // �����¼�����ʱ���������¼����ͻ�д��revents_��,Ȼ��Ϳ��Ը������ֵ������ִ���ĸ�������
    bool
        in_epoll_;    // ��ǰChannel�Ƿ��Ѿ���epoll���¼������������;(�����Ϊ��Channelһһ��Ӧ��fd�Ƿ��Ѿ���epoll���¼�����)
    std::function<void()> read_callback_;     // �ɶ��ص�����
    std::function<void()> write_callback_;    // ��д�ص�����
};

#endif    // NOLINT