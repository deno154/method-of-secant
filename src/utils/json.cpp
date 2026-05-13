#include "json.h"

std::string extractJsonValue(const std::string &json,
                             const std::string &key)
{

    std::string searchKey = "\"" + key + "\"";

    size_t keyPos = json.find(searchKey);

    if (keyPos == std::string::npos)
    {
        return "";
    }

    size_t colonPos = json.find(":", keyPos);

    if (colonPos == std::string::npos)
    {
        return "";
    }

    size_t startQuote = json.find("\"", colonPos + 1);

    if (startQuote == std::string::npos)
    {
        return "";
    }

    size_t endQuote = json.find("\"", startQuote + 1);

    if (endQuote == std::string::npos)
    {
        return "";
    }

    return json.substr(startQuote + 1,
                       endQuote - startQuote - 1);
}