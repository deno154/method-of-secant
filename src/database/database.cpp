#include "database.h"

#include <iostream>

Database *Database::instance = nullptr;

Database::Database()
{
    connected = false;
    userCount = 0;
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
    std::cout << "\n=== DATABASE ===\n";

    std::cout << "Host: " << host << std::endl;
    std::cout << "User: " << user << std::endl;
    std::cout << "Database: " << dbName << std::endl;

    connected = true;

    std::cout << "Connection status: SUCCESS\n";

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

bool Database::userExists(const std::string &login) const
{
    for (int i = 0; i < userCount; i++)
    {
        if (users[i].login == login)
        {
            return true;
        }
    }

    return false;
}

bool Database::addUser(const std::string &login,
                       const std::string &password)
{
    if (userExists(login))
    {
        return false;
    }

    if (userCount >= MAX_USERS)
    {
        return false;
    }

    users[userCount].login = login;
    users[userCount].password = password;

    userCount++;

    return true;
}

bool Database::validateUser(const std::string &login,
                            const std::string &password) const
{
    for (int i = 0; i < userCount; i++)
    {
        if (users[i].login == login &&
            users[i].password == password)
        {
            return true;
        }
    }

    return false;
}