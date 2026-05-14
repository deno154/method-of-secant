#include "server.h"
#include "../handlers/handlers.h"

#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

HttpServer::HttpServer(int p) : port(p) {}

void handleClient(int clientSocket)
{
    char buffer[8192] = {0};

    int bytesReceived = recv(clientSocket,
                             buffer,
                             sizeof(buffer),
                             0);

    if (bytesReceived <= 0)
    {
        close(clientSocket);
        return;
    }

    std::string request(buffer);

    std::cout << "\n=== REQUEST ===\n";
    std::cout << request << std::endl;

    std::string responseBody;

    // ROUTING
    if (request.find("POST /vigenere") != std::string::npos)
    {
        responseBody = handleVigenere(request);
    }
    else if (request.find("POST /md5") != std::string::npos)
    {
        responseBody = handleMD5(request);
    }
    else if (request.find("POST /secant") != std::string::npos)
    {
        responseBody = handleSecant(request);
    }
    else if (request.find("POST /graph-cycle") != std::string::npos)
    {
        responseBody = handleGraphCycle(request);
    }
    else
    {
        responseBody = R"({"status":"error","message":"Unknown endpoint"})";
    }

    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Content-Length: " +
        std::to_string(responseBody.size()) +
        "\r\n\r\n" +
        responseBody;

    send(clientSocket,
         httpResponse.c_str(),
         httpResponse.size(),
         0);

#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}

void HttpServer::start()
{
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0)
    {
        std::cerr << "Socket creation error\n";
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket,
             (sockaddr *)&serverAddr,
             sizeof(serverAddr)) < 0)
    {
        std::cerr << "Bind error\n";
        return;
    }

    if (listen(serverSocket, 5) < 0)
    {
        std::cerr << "Listen error\n";
        return;
    }

    std::cout << "Server started on port " << port << std::endl;

    while (true)
    {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);

        int clientSocket = accept(serverSocket,
                                  (sockaddr *)&clientAddr,
                                  &clientLen);

        if (clientSocket < 0)
        {
            std::cerr << "Accept error\n";
            continue;
        }

        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

#ifdef _WIN32
    closesocket(serverSocket);
    WSACleanup();
#else
    close(serverSocket);
#endif
}