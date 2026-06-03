#ifndef DATABASE_H
#define DATABASE_H

#include <string>

struct User
{
    std::string login;
    std::string password;
};

class Database
{
private:
    static Database *instance;

    Database();

    bool connected;

    static const int MAX_USERS = 100;

    User users[MAX_USERS];

    int userCount;

public:
    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;

    static Database &getInstance();

    bool connect(const std::string &host,
                 const std::string &user,
                 const std::string &password,
                 const std::string &dbName);

    void disconnect();

    bool isConnected() const;

    bool addUser(const std::string &login,
                 const std::string &password);

    bool userExists(const std::string &login) const;

    bool validateUser(const std::string &login,
                      const std::string &password) const;
};

#endif