#include "auth.h"

#include <iostream>

std::string registerUser(const std::string &request)
{
    std::cout << "[AUTH] register\n";

    return "{\"status\":\"ok\",\"message\":\"user registered (stub)\"}";
}

std::string loginUser(const std::string &request)
{
    std::cout << "[AUTH] login\n";

    return "{\"status\":\"ok\",\"message\":\"user logged in (stub)\"}";
}