#include "include/acceptor.h"

Acceptor::Acceptor(EventLoop *event_loop) : event_loop_(event_loop)
{
    listen_socket_    = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 8888);
    listen_socket_->Bind(addr);
    listen_socket_->Listen();
    // listen_socket_->SetNonBlocking(); // 在day11中修改为阻塞式

    accept_channel_                = new Channel(event_loop_, listen_socket_->GetFd());
    std::function<void()> callback = std::bind(&Acceptor::AcceptConnection, this);
    accept_channel_->SetReadCallback(callback);
    accept_channel_->EnableReadEvents();

    delete addr;
    addr = nullptr;
}

Acceptor::~Acceptor()
{
    if (accept_channel_ != nullptr)
    {
        delete accept_channel_;
        accept_channel_ = nullptr;
    }
    if (listen_socket_ != nullptr)
    {
        delete listen_socket_;
        listen_socket_ = nullptr;
    }
}

void Acceptor::AcceptConnection()
{
    InetAddress *client_addr = new InetAddress();
    Socket *client_socket =
        new Socket(listen_socket_->Accept(client_addr));    // 该client_socket会在Connection的析构函数中释放
    printf("new client fd[%d]! IP: %s Port: %d\n", client_socket->GetFd(), inet_ntoa(client_addr->address_.sin_addr),
           ntohs(client_addr->address_.sin_port));
    client_socket->SetNonBlocking();

    new_conn_callback_(client_socket);

    delete client_addr;
    client_addr = nullptr;
}

void Acceptor::SetNewConnCallback(std::function<void(Socket *)> callback)
{
    new_conn_callback_ = callback;
}