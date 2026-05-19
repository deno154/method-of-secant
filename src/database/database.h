#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <map>

class Database
{
private:
    static Database *instance;

    std::map<std::string, std::string> users;

    Database();

public:
    static Database &getInstance();

    bool addUser(const std::string &username,
                 const std::string &password);

    bool checkUser(const std::string &username,
                   const std::string &password);

    void printUsers();
};

#endif