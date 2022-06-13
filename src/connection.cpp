#include "include/connection.h"

Connection::Connection(EventLoop *event_loop, Socket *socket) :
    event_loop_(event_loop),
    client_socket_(socket),
    client_channel_(nullptr),
    state_(State::kInvalid),
    read_buffer_(nullptr),
    send_buffer_(nullptr)
{
    if (event_loop_ != nullptr)
    {
        client_channel_ = new Channel(event_loop_, client_socket_->GetFd());
        client_channel_->EnableReadEvents();
        client_channel_->UseET();
    }

    read_buffer_ = new Buffer();
    send_buffer_ = new Buffer();
    state_       = State::kConnected;
}

Connection::~Connection()
{
    if (client_channel_ != nullptr)
    {
        delete client_channel_;
        client_channel_ = nullptr;
    }
    if (client_socket_ != nullptr)    // 在Acceptor::AcceptConnection()中new，在这里释放
    {
        delete client_socket_;
        client_socket_ = nullptr;
    }
    if (read_buffer_ != nullptr)
    {
        delete read_buffer_;
        read_buffer_ = nullptr;
    }
    if (send_buffer_ != nullptr)
    {
        delete send_buffer_;
        send_buffer_ = nullptr;
    }
}

void Connection::SetDelConnCallback(std::function<void(Socket *)> const &callback)
{
    del_conn_callback_ = callback;
}

void Connection::SetOnConnectCallback(std::function<void(Connection *)> const &callback)
{
    on_connect_callback_ = callback;
    client_channel_->SetReadCallback([this]() { on_connect_callback_(this); });
}

void Connection::Read()
{
    ASSERT(state_ == State::kConnected, "connection state is disconnected!");
    read_buffer_->Clear();
    if (client_socket_->IsNonBlocking())
    {
        ReadNonBlocking();
    }
    else
    {
        ReadBlocking();
    }
}

void Connection::Write()
{
    ASSERT(state_ == State::kConnected, "connection state is disconnected!");
    if (client_socket_->IsNonBlocking())
    {
        WriteNonBlocking();
    }
    else
    {
        WriteBlocking();
    }
    send_buffer_->Clear();
}

void Connection::ReadNonBlocking()
{
    int sockfd     = client_socket_->GetFd();
    char buf[1024] = {0};
    // 使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    while (true)
    {
        memset(buf, 0, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            read_buffer_->Append(buf, read_bytes);
        }
        else if (read_bytes == -1 && errno == EINTR)    // 程序正常中断、继续读取
        {
            std::cout << "continue reading" << std::endl;
            continue;
        }
        else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            // 非阻塞IO，该条件表示数据全部读取完毕
            break;
        }
        else if (read_bytes == 0)    // EOF, 客户端断开连接
        {
            std::cout << "read EOF, client fd:" << sockfd << " disconnected." << std::endl;
            state_ = State::kClosed;
            break;
        }
        else
        {
            std::cout << "Other error on client fd:" << sockfd << std::endl;
            state_ = State::kClosed;
            break;
        }
    }
}

void Connection::WriteNonBlocking()
{
    int sockfd                     = client_socket_->GetFd();
    char buf[send_buffer_->Size()] = {0};
    memcpy(buf, send_buffer_->ToStr(), send_buffer_->Size());
    int data_size = send_buffer_->Size();
    int data_left = data_size;
    while (data_left > 0)
    {
        ssize_t write_bytes = write(sockfd, buf + data_size - data_left, data_left);
        if (write_bytes == -1 && errno == EINTR)
        {
            std::cout << "continue writing" << std::endl;
            continue;
        }
        if (write_bytes == -1 && errno == EAGAIN)
        {
            break;
        }
        if (write_bytes == -1)
        {
            std::cout << "Other error on client fd:" << sockfd << std::endl;
            state_ = State::kClosed;
            break;
        }
        data_left -= write_bytes;
    }
}

// Never used by server, only for client
void Connection::ReadBlocking()
{
    int sockfd             = client_socket_->GetFd();
    unsigned int recv_size = 0;
    socklen_t length       = sizeof(recv_size);
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recv_size, &length);
    char buf[recv_size] = {0};
    ssize_t read_bytes  = read(sockfd, buf, sizeof(buf));
    if (read_bytes > 0)
    {
        read_buffer_->Append(buf, read_bytes);
    }
    else if (read_bytes == 0)
    {
        std::cout << "read EOF, blocking client fd:" << sockfd << " disconnected." << std::endl;
        state_ = State::kClosed;
    }
    else if (read_bytes == -1)
    {
        std::cout << "Other error on blocking client fd:" << sockfd << std::endl;
        state_ = State::kClosed;
    }
}

// Never used by server, only for client
void Connection::WriteBlocking()
{
    // 没有处理send_buffer_数据大于TCP写缓冲区的情况，可能会有bug
    int sockfd          = client_socket_->GetFd();
    ssize_t write_bytes = write(sockfd, send_buffer_->ToStr(), send_buffer_->Size());
    if (write_bytes == -1)
    {
        std::cout << "Other error on blocking client fd:" << sockfd << std::endl;
        state_ = State::kClosed;
    }
}

void Connection::Close()
{
    del_conn_callback_(client_socket_);
}

Connection::State Connection::GetState()
{
    return state_;
}

void Connection::SetSendBuffer(const char *str)
{
    send_buffer_->SetBuffer(str);
}

Buffer *Connection::GetSendBuffer()
{
    return send_buffer_;
}

Buffer *Connection::GetReadBuffer()
{
    return read_buffer_;
}

const char *Connection::SendBuffer()
{
    return send_buffer_->ToStr();
}

const char *Connection::ReadBuffer()
{
    return read_buffer_->ToStr();
}

void Connection::GetLineSendBuffer()
{
    send_buffer_->Getline();
}

Socket *Connection::GetSocket()
{
    return client_socket_;
}