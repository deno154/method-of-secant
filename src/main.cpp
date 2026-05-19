#include "server/server.h"
#include "database/database.h"

#include <iostream>

int main()
{
    Database &db =
        Database::getInstance();

    std::cout << "\n=== DATABASE READY ===\n";

    HttpServer server(8080);

    server.start();

    return 0;
}