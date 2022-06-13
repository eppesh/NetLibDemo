#include "include/socket.h"

Socket::Socket() : sockfd_(-1)
{
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    ErrorIf((sockfd_ == -1), "socket create error");
}

Socket::Socket(int sockfd) : sockfd_(sockfd)
{
    ErrorIf((sockfd_ == -1), "socket create error");
}

Socket::~Socket()
{
    if (sockfd_ != -1)
    {
        close(sockfd_);
        sockfd_ = -1;
    }
}

void Socket::Bind(InetAddress *address)
{
    sockaddr_in tmp_addr = address->GetAddr();
    ErrorIf(bind(sockfd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1, "socket bind error");
}

void Socket::Listen()
{
    ErrorIf(listen(sockfd_, SOMAXCONN) == -1, "socket listen error");
}

void Socket::SetNonBlocking()
{
    fcntl(sockfd_, F_SETFL, fcntl(sockfd_, F_GETFL) | O_NONBLOCK);
}

bool Socket::IsNonBlocking()
{
    return (fcntl(sockfd_, F_GETFL) & O_NONBLOCK) != 0;
}

int Socket::Accept(InetAddress *address)
{
    int client_sockfd = -1;
    sockaddr_in tmp_addr {};
    socklen_t addr_len = sizeof(tmp_addr);
    if (fcntl(sockfd_, F_GETFL) & O_NONBLOCK)
    {
        while (true)
        {
            client_sockfd = accept(sockfd_, (sockaddr *)&tmp_addr, &addr_len);
            if (client_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
            {
                continue;    // no connection yet
            }
            if (client_sockfd == -1)
            {
                ErrorIf(true, "socket accept error");
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        client_sockfd = accept(sockfd_, (sockaddr *)&tmp_addr, &addr_len);
        ErrorIf((client_sockfd == -1), "socket accept error");
    }
    address->SetInetAddr(tmp_addr);
    return client_sockfd;
}

int Socket::GetFd()
{
    return sockfd_;
}

// used for client socket
void Socket::Connect(InetAddress *address)
{
    sockaddr_in addr = address->GetAddr();
    if (fcntl(sockfd_, F_GETFL) & O_NONBLOCK)
    {
        while (true)
        {
            int ret = connect(sockfd_, (sockaddr *)&addr, sizeof(addr));
            if (ret == 0)
            {
                break;
            }
            if (ret == -1 && (errno == EINPROGRESS))
            {
                continue;
                /* 连接非阻塞式sockfd建议的做法：
                The socket is nonblocking and the connection cannot be
                completed immediately.  (UNIX domain sockets failed with
                EAGAIN instead.)  It is possible to select(2) or poll(2)
                for completion by selecting the socket for writing.  After
                select(2) indicates writability, use getsockopt(2) to read
                the SO_ERROR option at level SOL_SOCKET to determine
                whether connect() completed successfully (SO_ERROR is
                zero) or unsuccessfully (SO_ERROR is one of the usual
                error codes listed here, explaining the reason for the
                failure).
                这里为了简单、不断连接直到连接完成，相当于阻塞式
                */
            }
            if (ret == -1)
            {
                ErrorIf(true, "socket connect error");
            }
        }
    }
    else
    {
        ErrorIf((connect(sockfd_, (sockaddr *)&addr, sizeof(addr)) == -1), "socket connect error");
    }
}

void Socket::Connect(const char *ip, uint16_t port)
{
    InetAddress *addr = new InetAddress(ip, port);
    Connect(addr);
    delete addr;
    addr = nullptr;
}

InetAddress::InetAddress(const char *ip, uint16_t port)
{
    memset(&address_, 0, sizeof(address_));
    address_.sin_family      = AF_INET;
    address_.sin_addr.s_addr = inet_addr(ip);
    address_.sin_port        = htons(port);
}

void InetAddress::SetInetAddr(sockaddr_in address)
{
    address_ = address;
}

sockaddr_in InetAddress::GetAddr()
{
    return address_;
}

const char *InetAddress::GetIP()
{
    return inet_ntoa(address_.sin_addr);
}

uint16_t InetAddress::GetPort()
{
    return ntohs(address_.sin_port);
}