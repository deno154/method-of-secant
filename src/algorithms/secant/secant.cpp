#include "secant.h"
#include "../../utils/simple_json.h"
#include <cstdlib>

static double f(double x)
{
    return x * x - 4;
}

std::string secantStub(const std::string &body)
{
    std::string sa = extract(body, "a");
    std::string sb = extract(body, "b");

    double a = sa.empty() ? 1 : std::stod(sa);
    double b = sb.empty() ? 3 : std::stod(sb);

    double x0 = a, x1 = b, x2;

    for (int i = 0; i < 10; i++)
    {
        double f0 = f(x0);
        double f1 = f(x1);

        if (f1 - f0 == 0) break;

        x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
        x0 = x1;
        x1 = x2;
    }

    return "{\"status\":\"ok\",\"algorithm\":\"secant\",\"root\":\"" + std::to_string(x1) + "\"}";
}