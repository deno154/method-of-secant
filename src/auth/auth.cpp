#include "auth.h"
#include "../database/database.h"

#include <iostream>
#include <string>

static std::string extractValue(
    const std::string &json,
    const std::string &key)
{
    std::string pattern =
        "\"" + key + "\":\"";

    size_t start =
        json.find(pattern);

    if (start == std::string::npos)
    {
        return "";
    }

    start += pattern.length();

    size_t end =
        json.find("\"", start);

    if (end == std::string::npos)
    {
        return "";
    }

    return json.substr(start, end - start);
}

std::string registerUser(const std::string &request)
{
    Database &db =
        Database::getInstance();

    std::string login =
        extractValue(request, "user");

    std::string password =
        extractValue(request, "password");

    std::cout
        << "[AUTH] register: "
        << login
        << std::endl;

    if (login.empty() || password.empty())
    {
        return R"({"status":"error","message":"invalid request"})";
    }

    if (db.userExists(login))
    {
        std::cout
            << "[AUTH] user already exists: "
            << login
            << std::endl;

        return R"({"status":"error","message":"user already exists"})";
    }

    if (!db.addUser(login, password))
    {
        return R"({"status":"error","message":"registration failed"})";
    }

    std::cout
        << "[AUTH] user registered: "
        << login
        << std::endl;

    return R"({"status":"ok","message":"user registered"})";
}

std::string loginUser(const std::string &request)
{
    Database &db =
        Database::getInstance();

    std::string login =
        extractValue(request, "user");

    std::string password =
        extractValue(request, "password");

    std::cout
        << "[AUTH] login: "
        << login
        << std::endl;

    if (login.empty() || password.empty())
    {
        return R"({"status":"error","message":"invalid request"})";
    }

    if (!db.validateUser(login, password))
    {
        std::cout
            << "[AUTH] login failed: "
            << login
            << std::endl;

        return R"({"status":"error","message":"invalid login or password"})";
    }

    std::cout
        << "[AUTH] login success: "
        << login
        << std::endl;

    return R"({"status":"ok","message":"user logged in"})";
}