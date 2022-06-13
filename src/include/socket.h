// Name: socket.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/26/2022
// Description:Socket��; ��������socket����

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

    void SetInetAddr(sockaddr_in address);    // ���������ַ
    sockaddr_in GetAddr();
    const char *GetIP();
    uint16_t GetPort();

public:
    sockaddr_in address_ {};
};

class Socket
{
public:
    Socket();                       // Ĭ�Ϲ��캯����ʾ���½�listenfd,����ֵ��sockfd_
    explicit Socket(int sockfd);    // ���ι��캯����ʾ�ǿͻ��˵�clientfd,����ֵ��sockfd_
    ~Socket();

    DISALLOW_COPY_AND_MOVE(Socket);

    void Bind(InetAddress *address);     // ��IP��Port
    void Listen();                       // ����
    int Accept(InetAddress *address);    // ��������,���ؿͻ���fd

    void SetNonBlocking();    // ����Ϊ������
    int GetFd();              // ��ȡfd
    bool IsNonBlocking();

    void Connect(InetAddress *address);    // ���ӷ����
    void Connect(const char *ip, uint16_t port);

private:
    int sockfd_;
};

#endif    // NOLINT