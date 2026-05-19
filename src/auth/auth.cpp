#include "auth.h"
#include "../database/database.h"

#include <string>
#include <iostream>

std::string extractValue(const std::string &request,
                         const std::string &key)
{
    std::string pattern = "\"" + key + "\":\"";

    size_t start = request.find(pattern);

    if (start == std::string::npos)
    {
        return "";
    }

    start += pattern.length();

    size_t end = request.find("\"", start);

    if (end == std::string::npos)
    {
        return "";
    }

    return request.substr(start,
                          end - start);
}

std::string registerUser(const std::string &request)
{
    std::string username =
        extractValue(request, "user");

    std::string password =
        extractValue(request, "password");

    if (username.empty() || password.empty())
    {
        return R"({
    "status":"error",
    "message":"invalid data"
})";
    }

    Database &db =
        Database::getInstance();

    bool success =
        db.addUser(username,
                   password);

    if (!success)
    {
        return R"({
    "status":"error",
    "message":"user already exists"
})";
    }

    return R"({
    "status":"ok",
    "message":"user registered"
})";
}

std::string loginUser(const std::string &request)
{
    std::string username =
        extractValue(request, "user");

    std::string password =
        extractValue(request, "password");

    Database &db =
        Database::getInstance();

    bool success =
        db.checkUser(username,
                     password);

    if (!success)
    {
        return R"({
    "status":"error",
    "message":"invalid login or password"
})";
    }

    return R"({
    "status":"ok",
    "message":"login successful"
})";
}