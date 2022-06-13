#include "include/event_loop.h"

EventLoop::EventLoop() : epoll_fd_(nullptr), is_quit_(false)
{
    epoll_fd_ = new Epoll();
    ErrorIf((epoll_fd_ == nullptr), "epoll new error");
}

EventLoop::~EventLoop()
{
    if (epoll_fd_ != nullptr)
    {
        delete epoll_fd_;
        epoll_fd_ = nullptr;
    }
}

void EventLoop::Loop()
{
    while (!is_quit_)
    {
        std::vector<Channel *> channels;
        channels = epoll_fd_->Poll();
        for (auto it = channels.begin(); it != channels.end(); ++it)
        {
            (*it)->HandleEvents();
        }
    }
}

void EventLoop::UpdateChannel(Channel *channel)
{
    epoll_fd_->UpdateChannel(channel);
}
