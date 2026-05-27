#ifndef SIMPLE_JSON_H
#define SIMPLE_JSON_H

#include <string>

static inline std::string extract(const std::string &body, const std::string &key)
{
    std::string pattern = "\"" + key + "\"";

    size_t p = body.find(pattern);
    if (p == std::string::npos) return "";

    size_t colon = body.find(":", p);
    if (colon == std::string::npos) return "";

    size_t q1 = body.find("\"", colon);
    if (q1 == std::string::npos) return "";

    size_t q2 = body.find("\"", q1 + 1);
    if (q2 == std::string::npos) return "";

    return body.substr(q1 + 1, q2 - q1 - 1);
}

#endif