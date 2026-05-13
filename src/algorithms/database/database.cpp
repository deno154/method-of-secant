#include "database.h"

#include <iostream>

Database *Database::instance = nullptr;

Database::Database()
{
    connected = false;
}

Database &Database::getInstance()
{

    if (instance == nullptr)
    {
        instance = new Database();
    }

    return *instance;
}

bool Database::connect(const std::string &host,
                       const std::string &user,
                       const std::string &password,
                       const std::string &dbName)
{

    std::cout << "Connecting to database...\n";

    connected = true;

    std::cout << "Database connected\n";

    return true;
}

void Database::disconnect()
{

    connected = false;

    std::cout << "Database disconnected\n";
}

bool Database::isConnected() const
{
    return connected;
}