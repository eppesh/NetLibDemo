// ���ƣ�single_client.cpp
// Copyright������ѧϰ
// ���ߣ�Sean (eppesh@163.com)
// ������VS2019
// ʱ�䣺04/26/2022
// ˵�����ͻ���

#include "connection.h"
#include "socket.h"

#include <iostream>

int main()
{
    Socket *socket = new Socket();
    socket->Connect("127.0.0.1", 8888);

    Connection *conn = new Connection(nullptr, socket);

    while (true)
    {
        conn->GetLineSendBuffer();
        conn->Write();
        if (conn->GetState() == Connection::State::kClosed)
        {
            conn->Close();
            break;
        }
        conn->Read();
        std::cout << "Message from server: " << conn->ReadBuffer() << std::endl;
    }

    if (conn != nullptr)
    {
        delete conn;
        conn = nullptr;
    }

    if (socket != nullptr)
    {
        delete socket;
        socket = nullptr;
    }
    return 0;
}