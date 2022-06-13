#include "include/epoll.h"

Epoll::Epoll() : epoll_fd_(-1), active_events_(nullptr)
{
    epoll_fd_ = epoll_create1(0);
    ErrorIf((epoll_fd_ == -1), "epoll create error");
    active_events_ = new epoll_event[kMaxEvents];
    ErrorIf((active_events_ == nullptr), "new epoll_event error");
    memset(active_events_, 0, sizeof(*active_events_) * kMaxEvents);
}

Epoll::~Epoll()
{
    if (epoll_fd_ != -1)
    {
        close(epoll_fd_);
        epoll_fd_ = -1;
    }
    if (active_events_ != nullptr)
    {
        delete[] active_events_;
        active_events_ = nullptr;
    }
}

void Epoll::AddFd(int fd, uint32_t events)
{
    epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events  = events;
    ErrorIf((epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1), "epoll add event error");
}

std::vector<Channel *> Epoll::Poll(int timeout /* = -1 */)
{
    std::vector<Channel *> active_channels;
    int nfds = epoll_wait(epoll_fd_, active_events_, kMaxEvents, timeout);
    ErrorIf((nfds == -1), "epoll wait error");

    for (int i = 0; i < nfds; ++i)
    {
        Channel *channel = (Channel *)active_events_[i].data.ptr;
        channel->SetRevents(active_events_[i].events);
        active_channels.push_back(channel);
    }
    return active_channels;
}

void Epoll::UpdateChannel(Channel *channel)
{
    int fd = channel->GetFd();
    epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.ptr = channel;
    ev.events   = channel->GetEvents();
    if (!channel->GetInEpoll())    // 该channel/fd还不在epoll的事件表中(还未注册)
    {
        ErrorIf((epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1), "epoll add error");
        channel->SetInEpoll();
    }
    else
    {
        ErrorIf((epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev) == -1), "epoll modify error");
    }
}