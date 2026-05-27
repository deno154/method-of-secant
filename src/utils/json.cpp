#include "json.h"

std::string extractJsonValue(const std::string &json,
                             const std::string &key)
{
    std::string pattern = "\"" + key + "\"";

    size_t p = json.find(pattern);
    if (p == std::string::npos) return "";

    size_t colon = json.find(":", p);
    if (colon == std::string::npos) return "";

    size_t q1 = json.find("\"", colon);
    if (q1 == std::string::npos) return "";

    size_t q2 = json.find("\"", q1 + 1);
    if (q2 == std::string::npos) return "";

    return json.substr(q1 + 1, q2 - q1 - 1);
}