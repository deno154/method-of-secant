#ifndef SERVER_H
#define SERVER_H

class HttpServer
{
private:
    int port;

public:
    HttpServer(int p);
    void start();
};

#endif