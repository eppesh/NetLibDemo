// Name: server.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/28/2022
// Description:Server类; 把服务器封装成一个Server类

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

    void NewConnection(Socket *listen_socket);    // 新建连接
    void DeleteConnection(Socket *sock);          // 删除连接
    void OnConnect(std::function<void(Connection *)> callback);

private:
    EventLoop *main_reactor_;                    // main Reactor 只负责接受连接，然后分发给一个sub Reactor
    Acceptor *acceptor_;                         // 连接接受器，只负责接受新连接
    std::map<int, Connection *> connections_;    // 每个Connection对应一个TCP连接
    std::vector<EventLoop *>
        sub_reactors_;    // sub Reactor 负责处理事件循环：每个sub Reactor 对应一个线程，负责某类Connection上的事件循环
    ThreadPool *thread_pool_;    // 线程池
    std::function<void(Connection *)> on_connect_callback_;
};

#endif    // NOLINT