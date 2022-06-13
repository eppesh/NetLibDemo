// Name: acceptor.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 05/02/2022
// Description:Acceptor 类, 用于创建连接

#ifndef ACCEPTOR_H_    // NOLINT
#define ACCEPTOR_H_    // NOLINT

#include "epoll.h"
#include "event_loop.h"
#include "macros.h"
#include "socket.h"
#include <sys/epoll.h>

#include <functional>

class Epoll;
class EventLoop;
class InetAddress;
class Socket;

class Acceptor
{
public:
    explicit Acceptor(EventLoop *event_loop);
    ~Acceptor();

    DISALLOW_COPY_AND_MOVE(Acceptor);

    void AcceptConnection();
    void SetNewConnCallback(std::function<void(Socket *)> callback);

private:
    EventLoop *event_loop_;
    Socket *listen_socket_;
    Channel *accept_channel_;
    std::function<void(Socket *)> new_conn_callback_;
};
#endif    // NOLINT