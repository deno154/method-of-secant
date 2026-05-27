#include "md5.h"
#include "../../utils/simple_json.h"

static unsigned int hashSimple(const std::string &s)
{
    unsigned int h = 5381;
    for (char c : s)
        h = ((h << 5) + h) + c;
    return h;
}

std::string md5Stub(const std::string &body)
{
    std::string text = extract(body, "text");

    if (text.empty())
        return "{\"status\":\"error\",\"message\":\"invalid json\"}";

    unsigned int h = hashSimple(text);

    return "{\"status\":\"ok\",\"algorithm\":\"md5\",\"hash\":\"" + std::to_string(h) + "\"}";
}