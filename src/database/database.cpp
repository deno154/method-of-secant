#include "database.h"

#include <iostream>

Database *Database::instance = nullptr;

Database::Database()
{
    std::cout << "\n=== DATABASE INITIALIZED ===\n";
}

Database &Database::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Database();
    }

    return *instance;
}

bool Database::addUser(const std::string &username,
                       const std::string &password)
{
    if (users.find(username) != users.end())
    {
        return false;
    }

    users[username] = password;

    std::cout << "[DB] User added: "
              << username
              << std::endl;

    return true;
}

bool Database::checkUser(const std::string &username,
                         const std::string &password)
{
    auto it = users.find(username);

    if (it == users.end())
    {
        return false;
    }

    return it->second == password;
}

void Database::printUsers()
{
    std::cout << "\n=== USERS ===\n";

    for (const auto &user : users)
    {
        std::cout << user.first
                  << " : "
                  << user.second
                  << std::endl;
    }
}