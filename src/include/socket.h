// Name: socket.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/26/2022
// Description:Socket类; 包含常用socket操作

#ifndef SOCKET_H_    // NOLINT
#define SOCKET_H_    // NOLINT

#include "macros.h"
#include "util.h"
#include <arpa/inet.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

class InetAddress
{
public:
    InetAddress() = default;
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress() = default;

    DISALLOW_COPY_AND_MOVE(InetAddress);

    void SetInetAddr(sockaddr_in address);    // 设置网络地址
    sockaddr_in GetAddr();
    const char *GetIP();
    uint16_t GetPort();

public:
    sockaddr_in address_ {};
};

class Socket
{
public:
    Socket();                       // 默认构造函数表示是新建listenfd,并赋值给sockfd_
    explicit Socket(int sockfd);    // 带参构造函数表示是客户端的clientfd,并赋值给sockfd_
    ~Socket();

    DISALLOW_COPY_AND_MOVE(Socket);

    void Bind(InetAddress *address);     // 绑定IP和Port
    void Listen();                       // 监听
    int Accept(InetAddress *address);    // 接受连接,返回客户端fd

    void SetNonBlocking();    // 设置为非阻塞
    int GetFd();              // 获取fd
    bool IsNonBlocking();

    void Connect(InetAddress *address);    // 连接服务端
    void Connect(const char *ip, uint16_t port);

private:
    int sockfd_;
};

#endif    // NOLINT