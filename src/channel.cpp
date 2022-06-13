#include "include/channel.h"

Channel::Channel(EventLoop *event_loop, int fd) :
    event_loop_(event_loop), fd_(fd), events_(0), revents_(0), in_epoll_(false)
{
}

Channel::~Channel()
{
    if (fd_ != -1)
    {
        close(fd_);
        fd_ = -1;
    }
}

void Channel::EnableReadEvents()
{
    events_ |= EPOLLIN | EPOLLPRI;
    event_loop_->UpdateChannel(this);
}

void Channel::HandleEvents()
{
    if (revents_ & (EPOLLIN | EPOLLPRI))
    {
        read_callback_();
    }
    if (revents_ & (EPOLLOUT))
    {
        write_callback_();
    }
}

int Channel::GetFd()
{
    return fd_;
}

uint32_t Channel::GetEvents()
{
    return events_;
}

uint32_t Channel::GetRevents()
{
    return revents_;
}

void Channel::SetRevents(uint32_t ev)
{
    revents_ = ev;
}

bool Channel::GetInEpoll()
{
    return in_epoll_;
}

void Channel::SetInEpoll()
{
    in_epoll_ = true;
}

void Channel::UseET()
{
    events_ |= EPOLLET;
    event_loop_->UpdateChannel(this);
}

void Channel::SetReadCallback(std::function<void()> read_callback)
{
    read_callback_ = read_callback;
}