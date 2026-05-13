#ifndef JSON_H
#define JSON_H

#include <string>

std::string extractJsonValue(const std::string &json,
                             const std::string &key);

#endif