#include "graph_cycle.h"
#include "../../utils/simple_json.h"
#include <vector>

static bool dfs(int v, int p, std::vector<std::vector<int>> &g, std::vector<bool> &vis)
{
    vis[v] = true;

    for (int to : g[v])
    {
        if (!vis[to])
        {
            if (dfs(to, v, g, vis)) return true;
        }
        else if (to != p)
        {
            return true;
        }
    }
    return false;
}

std::string graphCycleStub(const std::string &body)
{
    std::string edges = extract(body, "graph");

    if (edges.empty())
        return "{\"status\":\"error\",\"message\":\"invalid json\"}";

    std::vector<std::vector<int>> g(10);
    std::vector<bool> vis(10, false);

    for (int i = 0; i < (int)edges.size(); i++)
    {
        if (edges[i] >= '0' && edges[i] <= '9')
        {
            int a = edges[i] - '0';
            int b = edges[i + 2] - '0';
            g[a].push_back(b);
            g[b].push_back(a);
            i += 2;
        }
    }

    bool cycle = dfs(1, -1, g, vis);

    return std::string("{\"status\":\"ok\",\"algorithm\":\"graph_cycle\",\"cycle\":") +
           (cycle ? "true" : "false") + "}";
}