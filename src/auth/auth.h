#ifndef AUTH_H
#define AUTH_H

#include <string>

std::string registerUser(const std::string &request);
std::string loginUser(const std::string &request);

#endif