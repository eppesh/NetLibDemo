// ���ƣ�server.cpp
// Copyright������ѧϰ
// ���ߣ�Sean (eppesh@163.com)
// ������VS2019
// ʱ�䣺04/26/2022
// ˵���������

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
