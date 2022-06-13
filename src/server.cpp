#include "include/server.h"

Server::Server(EventLoop *main_reactor) : main_reactor_(main_reactor), acceptor_(nullptr), thread_pool_(nullptr)
{
    acceptor_                              = new Acceptor(main_reactor_);
    std::function<void(Socket *)> callback = std::bind(&Server::NewConnection, this, std::placeholders::_1);
    acceptor_->SetNewConnCallback(callback);

    int size     = std::thread::hardware_concurrency();    // 硬件支持的线程数
    thread_pool_ = new ThreadPool(size);
    for (int i = 0; i < size; ++i)
    {
        sub_reactors_.emplace_back(new EventLoop());
    }

    for (int i = 0; i < size; ++i)
    {
        std::function<void()> sub_loop = std::bind(&EventLoop::Loop, sub_reactors_[i]);
        thread_pool_->Add(std::move(sub_loop));
    }
}

Server::~Server()
{
    if (acceptor_ != nullptr)
    {
        delete acceptor_;
        acceptor_ = nullptr;
    }
    if (thread_pool_ != nullptr)
    {
        delete thread_pool_;
        thread_pool_ = nullptr;
    }
    // sub_reactors_中的对象应该在这里释放？
}

void Server::NewConnection(Socket *client_socket)
{
    ErrorIf(client_socket->GetFd() == -1, "new connection error");
    uint64_t random                        = client_socket->GetFd() % sub_reactors_.size();
    Connection *conn                       = new Connection(sub_reactors_[random], client_socket);
    std::function<void(Socket *)> callback = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
    conn->SetDelConnCallback(callback);
    conn->SetOnConnectCallback(on_connect_callback_);
    connections_[client_socket->GetFd()] = conn;
}

void Server::DeleteConnection(Socket *sock)
{
    int clientfd = sock->GetFd();
    auto it      = connections_.find(clientfd);
    if (it != connections_.end())
    {
        Connection *conn = connections_[clientfd];
        connections_.erase(clientfd);
        if (conn != nullptr)
        {
            delete conn;
            conn = nullptr;
        }
    }
}

void Server::OnConnect(std::function<void(Connection *)> callback)
{
    on_connect_callback_ = std::move(callback);
}