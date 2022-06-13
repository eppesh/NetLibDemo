// Name: server.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/28/2022
// Description:Server��; �ѷ�������װ��һ��Server��

#ifndef SERVER_H_    // NOLINT
#define SERVER_H_    // NOLINT

#include "acceptor.h"
#include "channel.h"
#include "connection.h"
#include "event_loop.h"
#include "macros.h"
#include "socket.h"
#include "thread_pool.h"
#include <sys/epoll.h>

#include <cstring>
#include <functional>
#include <map>
#include <unistd.h>

class Acceptor;
class Connection;
class EventLoop;
class Socket;

class Server
{
public:
    explicit Server(EventLoop *main_reactor);
    ~Server();

    DISALLOW_COPY_AND_MOVE(Server);

    void NewConnection(Socket *listen_socket);    // �½�����
    void DeleteConnection(Socket *sock);          // ɾ������
    void OnConnect(std::function<void(Connection *)> callback);

private:
    EventLoop *main_reactor_;                    // main Reactor ֻ����������ӣ�Ȼ��ַ���һ��sub Reactor
    Acceptor *acceptor_;                         // ���ӽ�������ֻ�������������
    std::map<int, Connection *> connections_;    // ÿ��Connection��Ӧһ��TCP����
    std::vector<EventLoop *>
        sub_reactors_;    // sub Reactor �������¼�ѭ����ÿ��sub Reactor ��Ӧһ���̣߳�����ĳ��Connection�ϵ��¼�ѭ��
    ThreadPool *thread_pool_;    // �̳߳�
    std::function<void(Connection *)> on_connect_callback_;
};

#endif    // NOLINT