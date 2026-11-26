#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>

using namespace std;

struct Node {
    int id;
    int degree;
};

class RegisterAllocator {
private:
    int V;
    vector<vector<int>> adj;

public:
    RegisterAllocator(int V) : V(V) {
        adj.resize(V, vector<int>(V, 0));
    }

    void addConflict(int u, int v) {
        if (u >= 0 && u < V && v >= 0 && v < V && u != v) {
            adj[u][v] = 1;
            adj[v][u] = 1;
        }
    }

    int getDegree(int v) const {
        int d = 0;
        for (int i = 0; i < V; ++i) if (adj[v][i]) d++;
        return d;
    }

    bool isSafe(int v, const vector<int>& color_array, int c) {
        for (int i = 0; i < V; i++) {
            if (adj[v][i] && c == color_array[i]) return false;
        }
        return true;
    }

    bool backtrackingUtil(int m, vector<int>& color_array, int v) {
        if (v == V) return true;
        for (int c = 1; c <= m; c++) {
            if (isSafe(v, color_array, c)) {
                color_array[v] = c;
                if (backtrackingUtil(m, color_array, v + 1)) return true;
                color_array[v] = 0;
            }
        }
        return false;
    }

    // 获取精确色数
    int getChromaticNumber() {
        for (int m = 1; m <= V; ++m) {
            vector<int> color_array(V, 0);
            if (backtrackingUtil(m, color_array, 0)) {
                return m;
            }
        }
        return V;
    }

    // --- Welsh-Powell 算法 ---
    int runWelshPowell() {
        vector<Node> nodes(V);
        for (int i = 0; i < V; ++i) {
            nodes[i].id = i;
            nodes[i].degree = getDegree(i);
        }
        sort(nodes.begin(), nodes.end(), [](const Node& a, const Node& b) {
            return a.degree > b.degree;
            });

        vector<int> color_array(V, 0);
        int color_k = 1;
        int colored_count = 0;

        while (colored_count < V) {
            for (int i = 0; i < V; ++i) {
                int u = nodes[i].id;
                if (color_array[u] != 0) continue;

                bool can_color = true;
                for (int j = 0; j < V; ++j) {
                    if (color_array[j] == color_k && adj[u][j]) {
                        can_color = false;
                        break;
                    }
                }
                if (can_color) {
                    color_array[u] = color_k;
                    colored_count++;
                }
            }
            color_k++;
        }
        return color_k - 1;
    }
};

void runOptimalityTest(int N) {
    cout << "Generating Graph for N=" << N << "..." << endl;
    RegisterAllocator graph(N);

    mt19937 gen(42);
    uniform_int_distribution<> dis(0, 100);

    int threshold = (N >= 30) ? 30 : 50;

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (dis(gen) < threshold) {
                graph.addConflict(i, j);
            }
        }
    }

    cout << "  >> Calculating Welsh-Powell (Approx)..." << endl;
    int k_wp = graph.runWelshPowell();

    cout << "  >> Calculating Exact Backtracking (Optimal)..." << endl;
    int k_exact = graph.getChromaticNumber();

    cout << "\n[RESULT TABLE ROW]" << endl;
    cout << "N | Exact Chi(G) | WP k | Diff" << endl;
    cout << N << " | " << k_exact << "            | " << k_wp << "    | +" << (k_wp - k_exact) << endl;
    cout << "------------------------------------" << endl;
}

int main() {
    vector<int> cases = { 10, 20, 30 };

    cout << "=== Optimality Analysis Benchmark ===" << endl;
    for (int n : cases) {
        runOptimalityTest(n);
    }

    return 0;
}