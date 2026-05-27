#include "vigenere.h"

static char shiftChar(char c, char k)
{
    if (c < 'a' || c > 'z') return c;
    if (k < 'a' || k > 'z') return c;

    return char('a' + (c - 'a' + (k - 'a')) % 26);
}

static std::string extract(const std::string &s, const std::string &key)
{
    std::string pattern = "\"" + key + "\"";
    size_t p = s.find(pattern);
    if (p == std::string::npos) return "";

    size_t colon = s.find(":", p);
    if (colon == std::string::npos) return "";

    size_t firstQuote = s.find("\"", colon);
    if (firstQuote == std::string::npos) return "";

    size_t secondQuote = s.find("\"", firstQuote + 1);
    if (secondQuote == std::string::npos) return "";

    return s.substr(firstQuote + 1, secondQuote - firstQuote - 1);
}

std::string vigenereStub(const std::string &body)
{
    std::string text = extract(body, "text");
    std::string key  = extract(body, "key");

    if (text.empty() || key.empty())
    {
        return R"({"status":"error","message":"invalid json"})";
    }

    std::string res = text;

    int j = 0;
    for (int i = 0; i < (int)text.size(); i++)
    {
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            res[i] = shiftChar(text[i], key[j % key.size()]);
            j++;
        }
    }

    return R"({"status":"ok","algorithm":"vigenere","result":")"
           + res +
           R"("})";
}