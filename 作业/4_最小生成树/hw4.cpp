#include<iostream>
#include <vector>
#include<limits>
#include<conio.h>
#include<algorithm>

#define MAX_VERTEX_NUM 26
#define MAX_COST INT_MAX
#define MEMORY_ALLOCATION_ERROR -1
#define INVAVID_PARENT_NODE -1


int inputInteger(int minNum, int maxNum, const char* prompt = "") {
	
	double num;
	while (true) {
		if (prompt != "")
			std::cout << "请输入" << prompt << "[范围" << minNum << "-" << maxNum << "]:";
		std::cin >> num;
		if (std::cin.fail()||static_cast<int>(num)!= num|| num < minNum || num > maxNum) {
			std::cout << "输入非法,请重新输入!" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else
			return num;
	}
}


struct Edge {
	int start;
	int end;
	int weight;
	bool operator<(const Edge& other) const {
		return this->weight < other.weight;
	}
};


class MininumSpanningTree {
private:
	int** costMatrix;
	int vertexNum;
	int minTotalCost;
	std::vector<Edge> edges;
	std::vector<int>parents;
	std::vector<int> heights;
	int findSet(int v);
	void mergeSet(int v1, int v2);
public:
	MininumSpanningTree(const int v);
	~MininumSpanningTree();
	bool insertEdge(const int v1, const int v2, const int cost);
	void Kruskal();
	int getMinTotalCost();
};

int MininumSpanningTree::findSet(int v) {
	if (parents[v] < 0)
		return v;
	else
		findSet(parents[v]);
}

void MininumSpanningTree::mergeSet(int v1, int v2) {
	int root1 = findSet(v1), root2 = findSet(v2);
	if (heights[root1] > heights[root2])
		parents[root2] = root1;
	else if (heights[root1] < heights[root2])
		parents[root1] = root2;
	else if (parents[root1] == parents[root2]) {
		parents[root2] = root1;
		heights[root1]++;
	}
}

MininumSpanningTree::MininumSpanningTree(const int v) {
	vertexNum = v;
	minTotalCost = 0;
	parents.resize(vertexNum, INVAVID_PARENT_NODE);
	costMatrix = new(std::nothrow) int* [vertexNum];
	if (costMatrix == NULL) {
		std::cerr << "Error: Memory allocation failed." << std::endl;
		exit(MEMORY_ALLOCATION_ERROR);
	}
	for (int i = 0; i < vertexNum; i++) {
		costMatrix[i] = new(std::nothrow) int[vertexNum];
		if (costMatrix[i] == NULL) {
			std::cerr << "Error: Memory allocation failed." << std::endl;
			exit(MEMORY_ALLOCATION_ERROR);
		}
		for (int j = 0; j < vertexNum; j++) {
			costMatrix[i][j] = 0;
		}
	}

	std::cout << std::endl;
	std::cout << ">>> 城市通信道路网节点";
	for (int i = 0; i < vertexNum; i++) {
		std::cout << static_cast<char>(i + 'A') << " ";
		
	}
	std::cout << "创建成功!" << std::endl << std::endl;

	/*input edge distance*/
	std::cout << ">>> 请输入电网各节点之间的距离,没有路则输入上限" << MAX_COST << std::endl;
	for (size_t i = 0; i < vertexNum; i++) {
		for (size_t j = i + 1; j < vertexNum; j++) {
			std::cout << "请输入城市通信网节点" << static_cast<char>(i + 'A') << "到" << static_cast<char>(j + 'A')
				<< "的边的距离[整数范围1-" << MAX_COST << "]:";
			int cost = inputInteger(1, MAX_COST);
			insertEdge(i, j, cost);
		}
	}
}

MininumSpanningTree::~MininumSpanningTree() {
	for (int i = 0; i < vertexNum; i++) {
		delete[] costMatrix[i];
	}
	delete[] costMatrix;
}

bool MininumSpanningTree::insertEdge(const int v1, const int v2, const int cost) {
	if (v1 < 0 || v1 >= vertexNum || v2 < 0 || v2 >= vertexNum || cost < 0 || cost > MAX_COST) {
		return false;
	}
	costMatrix[v1][v2] = costMatrix[v2][v1] = cost;
	edges.push_back({ v1,v2,cost });
	return true;
}

void MininumSpanningTree::Kruskal() {
	parents.resize(vertexNum,INVAVID_PARENT_NODE);
	heights.resize(vertexNum, 0);

	std::sort(edges.begin(), edges.end());
	int count = 0;
	std::vector<Edge>results;

	for (const auto& edge : edges) {
		int x = findSet(edge.start);
		int y = findSet(edge.end);
		if (x != y) {
			results.push_back(edge);
			mergeSet(x, y);
			count++;
			if (count == vertexNum - 1)
				break;
		}
	}
	
	std::cout << std::endl<<">>> 建立最小 Kruskal 生成树:" << std::endl;
	for (const auto& result : results) {
		std::cout << static_cast<char>(result.start + 'A') << "-<" << result.weight << ">-" << static_cast<char>(result.end + 'A') << std::endl;
		minTotalCost += result.weight;
	}
}


int MininumSpanningTree::getMinTotalCost() {
	return minTotalCost;
}


bool inputLogicalValue(char FalseValue = '0', char TrueValue = '1')
{
	while (true) {
		char InputChar = _getch();
		if (InputChar == 0 || InputChar == -32)
			InputChar = _getch();
		else if (InputChar == FalseValue || InputChar == TrueValue) {
			std::cout << InputChar << std::endl << std::endl;
			return InputChar == TrueValue;
		}
	}
}

int makeChoice(const char* prompt, int vertexNum)
{
	std::cout << prompt;
	while (true) {
		char choice = _getch();;
		if (choice == 0 || choice == 32)
			choice = _getch();
		if ('A' <= choice && choice <= static_cast<char>(vertexNum + 'A' - 1)) {
			std::cout << " [" << choice << "]" << std::endl << std::endl;
			return choice;
		}
	}
}

int main() {
	do {
		system("cls");
		std::cout << "+-----------------------+" << std::endl;
		std::cout << "|      最小生成树       |" << std::endl;
		std::cout << "| Minimum Spanning Tree |" << std::endl;
		std::cout << "+-----------------------+" << std::endl;

		int vertexNum = inputInteger(2, MAX_VERTEX_NUM, "城市通信网节点数");
		MininumSpanningTree mininumSpanningTree(vertexNum);

		mininumSpanningTree.Kruskal();

		std::cout << ">>> Kruskal 最小生成树的总权重为 " << mininumSpanningTree.getMinTotalCost() << std::endl << std::endl;

		std::cout << "是否退出程序 [y/n]: ";
	} while (!inputLogicalValue('n', 'y'));
	

	return 0;
}