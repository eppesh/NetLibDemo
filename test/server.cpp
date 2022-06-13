// 名称：server.cpp
// Copyright：仅供学习
// 作者：Sean (eppesh@163.com)
// 环境：VS2019
// 时间：04/26/2022
// 说明：服务端

#include "server.h"

#include "buffer.h"
#include "connection.h"
#include "event_loop.h"
#include "socket.h"

#include <iostream>

int main()
{
    EventLoop *event_loop = new EventLoop();
    Server *server        = new Server(event_loop);
    server->OnConnect([](Connection *conn) {
        conn->Read();
        if (conn->GetState() == Connection::State::kClosed)
        {
            conn->Close();
            return;
        }
        std::cout << "Message from client " << conn->GetSocket()->GetFd() << ": " << conn->ReadBuffer() << std::endl;
        conn->SetSendBuffer(conn->ReadBuffer());
        conn->Write();
    });
    event_loop->Loop();
    delete server;
    delete event_loop;
    return 0;
}
