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

    std::cout << "\n=== DATABASE ===" << std::endl;

    std::cout << "Host: " << host << std::endl;
    std::cout << "User: " << user << std::endl;
    std::cout << "Database: " << dbName << std::endl;

    connected = true;

    std::cout << "Connection status: SUCCESS" << std::endl;

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