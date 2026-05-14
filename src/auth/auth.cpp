#include "auth.h"
#include <iostream>

std::string registerUser(const std::string &request)
{
    std::cout << "[AUTH] register request\n";

    std::string response =
        "{\n"
        "  \"status\": \"ok\",\n"
        "  \"message\": \"user registered (stub)\"\n"
        "}";

    return response;
}

std::string loginUser(const std::string &request)
{
    std::cout << "[AUTH] login request\n";

    std::string response =
        "{\n"
        "  \"status\": \"ok\",\n"
        "  \"message\": \"user logged in (stub)\"\n"
        "}";

    return response;
}