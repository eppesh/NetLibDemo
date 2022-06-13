#include "buffer.h"
#include "connection.h"
#include "socket.h"
#include "thread_pool.h"
#include "util.h"

#include <cstring>
#include <functional>
#include <iostream>
#include <unistd.h>

void OneClient(int msgs, int wait)
{
    Socket *sock = new Socket();
    sock->Connect("127.0.0.1", 8888);
    Connection *conn = new Connection(nullptr, sock);

    sleep(wait);
    int count = 0;
    while (count < msgs)
    {
        conn->SetSendBuffer("I'm client!");
        conn->Write();
        if (conn->GetState() == Connection::State::kClosed)
        {
            conn->Close();
            break;
        }
        conn->Read();
        std::cout << "msg count " << count++ << ": " << conn->ReadBuffer() << std::endl;
    }

    if (conn != nullptr)
    {
        delete conn;
        conn = nullptr;
    }
}

int main(int argc, char *argv[])
{
    int threads           = 10;
    int msgs              = 50;
    int wait              = 0;
    int op                = -1;
    const char *optstring = "t:m:w:";
    while ((op = getopt(argc, argv, optstring)) != -1)
    {
        switch (op)
        {
        case 't':
            threads = std::stoi(optarg);
            break;
        case 'm':
            msgs = std::stoi(optarg);
            break;
        case 'w':
            wait = std::stoi(optarg);
            break;
        case '?':
            std::cout << "error optopt: " << optopt << std::endl;
            std::cout << "error opterr: " << opterr << std::endl;
            break;
        default:
            break;
        }
    }

    ThreadPool *pool           = new ThreadPool(threads);
    std::function<void()> func = std::bind(OneClient, msgs, wait);
    for (int i = 0; i < threads; ++i)
    {
        pool->Add(func);
    }
    if (pool != nullptr)
    {
        delete pool;
        pool = nullptr;
    }
    return 0;
}