#include "server.h"
#include "../handlers/handlers.h"
#include "../auth/auth.h"

#include <iostream>
#include <string>
#include <thread>
#include <cstring>

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

HttpServer::HttpServer(int p) : port(p) {}

void handleClient(int clientSocket)
{
    char buffer[8192];

    std::string request;

    // =========================
    // READ FULL REQUEST
    // =========================
    while (true)
    {
        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;

        request.append(buffer, bytes);

        // если уже получили headers
        size_t headerEnd = request.find("\r\n\r\n");
        if (headerEnd != std::string::npos)
        {
            // проверяем Content-Length
            size_t clPos = request.find("Content-Length:");
            if (clPos == std::string::npos)
                break;

            int contentLength = 0;
            sscanf(request.c_str() + clPos, "Content-Length: %d", &contentLength);

            int bodyStart = headerEnd + 4;
            int bodySize = request.size() - bodyStart;

            if (bodySize >= contentLength)
                break;
        }
    }

    std::cout << "\n=== REQUEST ===\n";
    std::cout << request << std::endl;

    // =========================
    // BODY EXTRACTION
    // =========================
    std::string body;

    size_t pos = request.find("\r\n\r\n");
    if (pos != std::string::npos)
    {
        body = request.substr(pos + 4);
    }

    std::cout << "\n===== DEBUG BODY =====\n";
    std::cout << "[" << body << "]\n";
    std::cout << "======================\n";

    // =========================
    // ROUTING
    // =========================
    std::string responseBody;

    if (request.find("POST /vigenere") != std::string::npos)
        responseBody = handleVigenere(body);

    else if (request.find("POST /md5") != std::string::npos)
        responseBody = handleMD5(body);

    else if (request.find("POST /secant") != std::string::npos)
        responseBody = handleSecant(body);

    else if (request.find("POST /graph-cycle") != std::string::npos)
        responseBody = handleGraphCycle(body);

    else if (request.find("POST /register") != std::string::npos)
        responseBody = registerUser(body);

    else if (request.find("POST /login") != std::string::npos)
        responseBody = loginUser(body);

    else
        responseBody = R"({"status":"error","message":"unknown endpoint"})";

    // =========================
    // RESPONSE
    // =========================
    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "Content-Length: " + std::to_string(responseBody.size()) + "\r\n\r\n" +
        responseBody;

    send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);

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

        std::thread(handleClient, clientSocket).detach();
    }

#ifdef _WIN32
    closesocket(serverSocket);
    WSACleanup();
#else
    close(serverSocket);
#endif
}