#include "server.h"
#include "../handlers/handlers.h"
#include "../auth/auth.h"

#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <atomic>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#ifdef _WIN32
typedef int socklen_t;
#endif

std::atomic<int> clientCounter(0);

HttpServer::HttpServer(int p)
    : port(p)
{
}

void handleClient(int clientSocket)
{
    int clientId = ++clientCounter;

    std::cout
        << "\n[CLIENT "
        << clientId
        << "] Connected\n";

    char buffer[8192];

    std::string request;

    while (true)
    {
        int bytes =
            recv(clientSocket,
                 buffer,
                 sizeof(buffer),
                 0);

        if (bytes <= 0)
        {
            break;
        }

        request.append(buffer, bytes);

        size_t headerEnd =
            request.find("\r\n\r\n");

        if (headerEnd != std::string::npos)
        {
            size_t clPos =
                request.find("Content-Length:");

            if (clPos == std::string::npos)
            {
                break;
            }

            int contentLength = 0;

            sscanf(
                request.c_str() + clPos,
                "Content-Length: %d",
                &contentLength);

            int bodyStart =
                static_cast<int>(headerEnd + 4);

            int bodySize =
                static_cast<int>(request.size()) - bodyStart;

            if (bodySize >= contentLength)
            {
                break;
            }
        }
    }

    std::cout << "\n=== REQUEST ===\n";
    std::cout << request << std::endl;

    std::string body;

    size_t bodyPos =
        request.find("\r\n\r\n");

    if (bodyPos != std::string::npos)
    {
        body =
            request.substr(bodyPos + 4);
    }

    std::cout
        << "\n===== DEBUG BODY =====\n";

    std::cout
        << "["
        << body
        << "]\n";

    std::cout
        << "======================\n";

    std::string responseBody;

    if (request.find("POST /vigenere") != std::string::npos)
    {
        std::cout
            << "[CLIENT "
            << clientId
            << "] /vigenere\n";

        responseBody =
            handleVigenere(body);
    }
    else if (request.find("POST /md5") != std::string::npos)
    {
        std::cout
            << "[CLIENT "
            << clientId
            << "] /md5\n";

        responseBody =
            handleMD5(body);
    }
    else if (request.find("POST /secant") != std::string::npos)
    {
        std::cout
            << "[CLIENT "
            << clientId
            << "] /secant\n";

        responseBody =
            handleSecant(body);
    }
    else if (request.find("POST /graph-cycle") != std::string::npos)
    {
        std::cout
            << "[CLIENT "
            << clientId
            << "] /graph-cycle\n";

        responseBody =
            handleGraphCycle(body);
    }
    else if (request.find("POST /register") != std::string::npos)
    {
        std::cout
            << "[CLIENT "
            << clientId
            << "] /register\n";

        responseBody =
            registerUser(body);
    }
    else if (request.find("POST /login") != std::string::npos)
    {
        std::cout
            << "[CLIENT "
            << clientId
            << "] /login\n";

        responseBody =
            loginUser(body);
    }
    else
    {
        std::cout
            << "[CLIENT "
            << clientId
            << "] Unknown endpoint\n";

        responseBody =
            R"({"status":"error","message":"unknown endpoint"})";
    }

    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
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

    std::cout
        << "[CLIENT "
        << clientId
        << "] Disconnected\n";
}

void HttpServer::start()
{
#ifdef _WIN32
    WSADATA wsa;

    WSAStartup(
        MAKEWORD(2, 2),
        &wsa);
#endif

    int serverSocket =
        socket(AF_INET,
               SOCK_STREAM,
               0);

    if (serverSocket < 0)
    {
        std::cerr
            << "Socket creation error\n";

        return;
    }

    sockaddr_in serverAddr{};

    serverAddr.sin_family =
        AF_INET;

    serverAddr.sin_addr.s_addr =
        INADDR_ANY;

    serverAddr.sin_port =
        htons(port);

    if (bind(serverSocket,
             (sockaddr *)&serverAddr,
             sizeof(serverAddr)) < 0)
    {
        std::cerr
            << "Bind error\n";

        return;
    }

    if (listen(serverSocket, 5) < 0)
    {
        std::cerr
            << "Listen error\n";

        return;
    }

    std::cout
        << "Server started on port "
        << port
        << std::endl;

    while (true)
    {
        sockaddr_in clientAddr{};

        socklen_t clientLen =
            sizeof(clientAddr);

        int clientSocket =
            accept(serverSocket,
                   (sockaddr *)&clientAddr,
                   &clientLen);

        if (clientSocket < 0)
        {
            std::cerr
                << "Accept error\n";

            continue;
        }

        std::thread(
            handleClient,
            clientSocket)
            .detach();
    }

#ifdef _WIN32
    closesocket(serverSocket);
    WSACleanup();
#else
    close(serverSocket);
#endif
}