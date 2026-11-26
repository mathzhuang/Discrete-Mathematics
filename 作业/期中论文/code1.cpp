#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

const int MAX_REGISTERS = 10;

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

	// 添加冲突边
	void addConflict(int u, int v) {
		if (u >= 0 && u < V && v >= 0 && v < V && u != v) {
			adj[u][v] = 1;
			adj[v][u] = 1;
		}
	}


	int getDegree(int v) const {
		int d = 0;
		for (int i = 0; i < V; ++i) {
			if (adj[v][i]) d++;
		}
		return d;
	}

	//  精确回溯算法

	bool isSafe(int v, const vector<int>& color_array, int c) {
		for (int i = 0; i < V; i++) {
			if (adj[v][i] && c == color_array[i]) {
				return false;
			}
		}
		return true;
	}

	// 递归求解
	bool backtrackingUtil(int m, vector<int>& color_array, int v) {
		if (v == V) return true;

		for (int c = 1; c <= m; c++) {
			if (isSafe(v, color_array, c)) {
				color_array[v] = c;

				if (backtrackingUtil(m, color_array, v + 1))
					return true;
				color_array[v] = 0;
			}
		}
		return false;
	}

	void runExactBacktracking(int m) {
		cout << "\n--- Exact Backtracking (m=" << m << ") ---" << endl;
		vector<int> color_array(V, 0);

		if (backtrackingUtil(m, color_array, 0)) {
			cout << "Solution Found: Allocation Successful." << endl;
			printSolution(color_array);
		}
		else {
			cout << "No Solution: Need more than " << m << " registers (Spill required)." << endl;
		}
	}

	//  Welsh-Powell 启发式算法

	void runWelshPowell() {
		cout << "\n--- Welsh-Powell Heuristic ---" << endl;
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
					// 如果节点 j 已经是当前颜色，并且 u 和 j 相
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

		cout << "Heuristic Allocation Completed." << endl;
		cout << "Total Registers Used (Chromatic Number Approx): " << color_k - 1 << endl;
		printSolution(color_array);
	}

	void printSolution(const vector<int>& color_array) {
		cout << "Variable ID | Physical Register (Color)" << endl;
		cout << "---------------------------------------" << endl;
		for (int i = 0; i < V; i++) {
			cout << "Var_" << i << setw(8) << "| " << color_array[i] << endl;
		}
	}
};

int main() {

	int num_variables = 5;
	RegisterAllocator allocator(num_variables);

	// 构建冲突图
	allocator.addConflict(0, 1);
	allocator.addConflict(0, 2);
	allocator.addConflict(1, 2);
	allocator.addConflict(1, 3);
	allocator.addConflict(2, 3);
	allocator.addConflict(3, 4);

	allocator.runExactBacktracking(3);
	allocator.runWelshPowell();

	return 0;
}