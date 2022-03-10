#include "TcpServer.h"
#include <cstdlib>
#include <memory>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "EventLoop.h"

using namespace std;
using namespace cannet;

void connecttioncallback(const TcpConnectionPtr &p)
{
    LOG << "connect";
}

void messagecallback(const TcpConnectionPtr &p,
                     Buffer *b)
{
    cout << b->retrieveAllAsString();
    auto temp = b->retrieveAllAsString();
    p->send(R"(HTTP/1.1 200 OK
Accept-Ranges: bytes
Cache-Control: max-age=0
Connection: keep-alive
Content-Length: 36
Content-Type: text/html
Date: Tue, 08 Mar 2022 09:46:38 GMT

GET / HTTP/1.1
Host: 127.0.0.1:8000
)");
    // cout << temp.size();
}

int main()
{
    struct sockaddr_in addr
    {
    };
    bzero(&addr, sizeof(addr));
    addr.sin_port = htons(stoi("8000"));
    addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0)
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    EventLoop loop;
    TcpServer server(&loop, addr);
    server.setConnectionCallback(connecttioncallback);
    server.setMessageCallback(messagecallback);
    server.setThreadNum(4);
    server.start();
    loop.loop();
}