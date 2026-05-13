#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Database
{
private:
    static Database *instance;

    Database();

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

private:
    bool connected;
};

#endif