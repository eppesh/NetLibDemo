// Name: connection.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 05/05/2022
// Description: Connection类,一个TCP连接类

#ifndef CONNECTION_H_    // NOLINT
#define CONNECTION_H_    // NOLINT

#undef NDEBUG

#include "buffer.h"
#include "channel.h"
#include "event_loop.h"
#include "macros.h"
#include "socket.h"
#include <sys/epoll.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <string>

class Connection
{
public:
    enum State
    {
        kInvalid = 1,
        kHandshaking,
        kConnected,
        kClosed,
        kFailed
    };

    Connection(EventLoop *event_loop, Socket *socket);
    ~Connection();

    DISALLOW_COPY_AND_MOVE(Connection);

    void Read();
    void Write();

    void SetDelConnCallback(std::function<void(Socket *)> const &callback);
    void SetOnConnectCallback(std::function<void(Connection *)> const &callback);
    State GetState();
    void Close();
    void SetSendBuffer(const char *str);
    Buffer *GetReadBuffer();
    const char *ReadBuffer();
    Buffer *GetSendBuffer();
    const char *SendBuffer();
    void GetLineSendBuffer();
    Socket *GetSocket();

    void OnConnect(std::function<void()> callback);

private:
    void ReadNonBlocking();
    void WriteNonBlocking();
    void ReadBlocking();
    void WriteBlocking();

private:
    EventLoop *event_loop_;
    Socket *client_socket_;
    Channel *client_channel_;
    State state_;

    std::function<void(Socket *)> del_conn_callback_;    // 删除连接的回调函数
    std::function<void(Connection *)> on_connect_callback_;
    Buffer *read_buffer_;
    Buffer *send_buffer_;
};
#endif    // NOLINT