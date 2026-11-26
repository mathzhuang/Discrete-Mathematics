#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath> 
#include<string>

using namespace std;

const int INF = 1e9;
const int MAX_CITIES = 20;

int getSafeIntInput() {
    double val;
    string remain; 

    while (true) {
        if (!(cin >> val)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入非法（非数字），请重新输入整数: ";
        }
        else {
            getline(cin, remain);
            bool isClean = true;
            for (char c : remain) {
                if (!isspace(c)) { 
                    isClean = false;
                    break;
                }
            }

            if (!isClean) {
                cout << "输入非法（数字后含乱码），请重新输入: ";
            }
            else if (val != static_cast<int>(val)) {
                cout << "输入非法（包含小数），请重新输入整数: ";
            }
            else {
                return static_cast<int>(val);
            }
        }
    }
}

struct TSPResult {
    int minCost;
    vector<int> path;
    bool success;
    string errorMessage;
};

class TSPSolver {
private:
    int n;
    vector<vector<int>> dist;
    vector<vector<int>> dp;
    vector<vector<int>> parent;

public:
    TSPSolver(int numCities) : n(numCities) {
        dist.resize(n, vector<int>(n));
    }

    void setDistance(int from, int to, int weight) {
        dist[from][to] = weight;
    }

    TSPResult solve() {
        TSPResult result;
        result.success = true;

        int stateCount = 1 << n;
        dp.assign(stateCount, vector<int>(n, INF));
        parent.assign(stateCount, vector<int>(n, -1));

        dp[1][0] = 0;

        for (int mask = 1; mask < stateCount; ++mask) {
            for (int i = 0; i < n; ++i) {
                if (!((mask >> i) & 1)) continue;
                if (dp[mask][i] == INF) continue;

                for (int next = 0; next < n; ++next) {
                    if ((mask >> next) & 1) continue;
                    if (dist[i][next] == INF) continue;

                    int nextMask = mask | (1 << next);
                    int newCost = dp[mask][i] + dist[i][next];

                    if (newCost < dp[nextMask][next]) {
                        dp[nextMask][next] = newCost;
                        parent[nextMask][next] = i;
                    }
                }
            }
        }

        int finalMask = stateCount - 1;
        int minCost = INF;
        int lastCity = -1;

        for (int i = 1; i < n; ++i) {
            if (dist[i][0] == INF) continue;
            if (dp[finalMask][i] != INF) {
                int currentTotal = dp[finalMask][i] + dist[i][0];
                if (currentTotal < minCost) {
                    minCost = currentTotal;
                    lastCity = i;
                }
            }
        }

        if (minCost == INF) {
            result.success = false;
            result.errorMessage = "无法遍历所有城市并回到起点（图不连通）。";
            return result;
        }

        result.minCost = minCost;

        int currMask = finalMask;
        int currCity = lastCity;
        while (currCity != -1) {
            result.path.push_back(currCity);
            int prevCity = parent[currMask][currCity];
            int prevMask = currMask ^ (1 << currCity);
            currCity = prevCity;
            currMask = prevMask;
        }
        reverse(result.path.begin(), result.path.end());
        result.path.push_back(0);

        return result;
    }
};

int main() {
    cout << "=== TSP问题求解器  ===" << endl;

    int n;
    while (true) {
        cout << "请输入城市数量 (2 - " << MAX_CITIES << "): ";
        n = getSafeIntInput();
        if (n >= 2 && n <= MAX_CITIES) break;
        cout << "错误：城市数量必须在 2 到 " << MAX_CITIES << " 之间。\n";
    }

    TSPSolver solver(n);

    cout << "\n请输入邻接矩阵 (" << n << "x" << n << ")。" << endl;
    cout << "规则：" << endl;
    cout << "1. 必须输入整数。输入小数(如3.5)将被视为非法。" << endl;
    cout << "2. 两个不同城市间的距离必须为正整数 (>0)。" << endl;
    cout << "3. 如果两个城市不可达，请输入 -1。" << endl;

    for (int i = 0; i < n; ++i) {
        cout << "请输入城市 " << i << " 到其他城市的距离:" << endl;
        for (int j = 0; j < n; ++j) {
            int w;

            if (i == j) {
                w = 0;
                cout << "  -> 到城市 " << j << ": 0 (自身)" << endl;
            }
            else {
                cout << "  -> 到城市 " << j << ": ";
                while (true) {
                    w = getSafeIntInput();

                    if (w > 0) {
                        break;
                    }
                    else if (w == -1) {
                        w = INF;
                        break;
                    }
                    else {
                        cout << "    [非法数值] 距离必须为正整数，或输入-1表示不可达。请重输: ";
                    }
                }
            }
            solver.setDistance(i, j, w);
        }
    }

    cout << "\n正在计算..." << endl;
    TSPResult res = solver.solve();

    if (res.success) {
        cout << "\n[成功]" << endl;
        cout << "最小路径总长: " << res.minCost << endl;
        cout << "路线: ";
        for (size_t i = 0; i < res.path.size(); ++i) {
            cout << res.path[i];
            if (i < res.path.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
    else {
        cout << "\n[失败] " << res.errorMessage << endl;
    }

    return 0;
}