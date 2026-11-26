#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <limits> 

using namespace std;

const int INF = 1e9;

int getSafeInt(const string& prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            if (val >= minVal && val <= maxVal) {
                return val;
            }
            else {
                cout << "错误：输入数值超出范围 [" << minVal << ", " << maxVal << "]，请重新输入。" << endl;
            }
        }
        else {
            cout << "错误：非法输入类型，请输入一个整数。" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

struct Edge {
    int u, v, weight;
};

class ChinesePostman {
private:
    int n;
    vector<vector<int>> dist;
    vector<int> degree;
    int total_weight;
    bool is_graph_connected; 

public:
    ChinesePostman(int nodes) : n(nodes), total_weight(0), is_graph_connected(true) {
        dist.resize(n, vector<int>(n, INF));
        for (int i = 0; i < n; ++i) dist[i][i] = 0;
        degree.resize(n, 0);
    }

    void addEdge(int u, int v, int w) {
        dist[u][v] = min(dist[u][v], w);
        dist[v][u] = min(dist[v][u], w);
        degree[u]++;
        degree[v]++;
        total_weight += w;
    }

    int solve() {
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        vector<int> odd_vertices;
        for (int i = 0; i < n; ++i) {
            if (degree[i] % 2 != 0) {
                odd_vertices.push_back(i);
            }
        }

        for (size_t i = 0; i < odd_vertices.size(); ++i) {
            for (size_t j = i + 1; j < odd_vertices.size(); ++j) {
                if (dist[odd_vertices[i]][odd_vertices[j]] == INF) {
                    cout << "错误：图不连通，奇度顶点之间无法到达，无法求解中国邮递员问题。" << endl;
                    return -1; 
                }
            }
        }

        if (odd_vertices.empty()) {
            cout << "该图已存在欧拉回路，无需重复走边。" << endl;
            return total_weight;
        }

        int k = odd_vertices.size();
        int max_mask = (1 << k);
        vector<int> dp(max_mask, INF);

        dp[0] = 0;

        for (int mask = 0; mask < max_mask; ++mask) {
            if (dp[mask] == INF) continue;

            int first_unmatched = -1;
            for (int i = 0; i < k; ++i) {
                if (!((mask >> i) & 1)) {
                    first_unmatched = i;
                    break;
                }
            }

            if (first_unmatched == -1) continue;

            for (int next_unmatched = first_unmatched + 1; next_unmatched < k; ++next_unmatched) {
                if (!((mask >> next_unmatched) & 1)) {
                    int new_mask = mask | (1 << first_unmatched) | (1 << next_unmatched);

                    int u = odd_vertices[first_unmatched];
                    int v = odd_vertices[next_unmatched];
                    int cost = dist[u][v];

                    if (cost != INF && dp[new_mask] > dp[mask] + cost) {
                        dp[new_mask] = dp[mask] + cost;
                    }
                }
            }
        }

        int min_matching_cost = dp[max_mask - 1];

        if (min_matching_cost == INF) {
            cout << "错误：无法找到有效的匹配路径（图可能不连通）。" << endl;
            return -1;
        }

        cout << "原始边总权重: " << total_weight << endl;
        cout << "奇度顶点数量: " << k << endl;
        cout << "为了形成回路，需要重复走的边的最小权重和: " << min_matching_cost << endl;

        return total_weight + min_matching_cost;
    }
};

int main() {

    cout << "=== 中国邮递员问题求解程序 ===" << endl;

    int n = getSafeInt("请输入顶点数 (2-20): ", 2, 20);
    int m = getSafeInt("请输入边数 (1-1000): ", 1, 1000);

    ChinesePostman solver(n);

    cout << "请依次输入每条边的起点、终点和权重 (u v w)" << endl;
    cout << "顶点编号范围: 0 到 " << n - 1 << endl;

    for (int i = 0; i < m; ++i) {
        cout << ">>> 第 " << i + 1 << " 条边: " << endl;

        int u = getSafeInt("    起点 u: ", 0, n - 1);
        int v = getSafeInt("    终点 v: ", 0, n - 1);

        if (u == v) {
            cout << "    (提示: 输入了自环边，将计入总权重但不影响奇偶性)" << endl;
        }

        int w = getSafeInt("    权重 w (1-10000): ", 1, 10000);

        solver.addEdge(u, v, w);
    }

    cout << "正在计算..." << endl;
    int result = solver.solve();

    cout << "--------------------------------" << endl;
    if (result != -1) {
        cout << "中国邮递员问题的最小路径总长: " << result << endl;
    }
    else {
        cout << "计算失败，请检查图的连通性。" << endl;
    }

    return 0;
}