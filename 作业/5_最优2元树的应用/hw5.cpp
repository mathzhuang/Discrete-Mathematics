#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<iomanip>
#include<conio.h>
#include<algorithm>

#define MEMORY_ALLOCATION_ERROR -1

struct HuffmanTreeNode {
	int num;
	std::string code;
	HuffmanTreeNode* leftChild, * rightChild;
	HuffmanTreeNode(int n) :num(n), leftChild(NULL), rightChild(NULL) {}

	bool operator<(const HuffmanTreeNode& otherNode)const { 
		return this->num < otherNode.num; 
	}
	friend std::ostream& operator<<(std::ostream& out, const HuffmanTreeNode& huffmanTreeNode) {
		out << std::setiosflags(std::ios::right) << std::setw(6) << huffmanTreeNode.num << ":" << huffmanTreeNode.code;
		return out;
	}
};

void createHuffmanTree(std::vector<HuffmanTreeNode> &nodes) {
	while (nodes.size() > 1) {
		std::sort(nodes.begin(), nodes.end());
		
		HuffmanTreeNode* tempParentNode = new HuffmanTreeNode(nodes[0].num + nodes[1].num);
		if (tempParentNode == NULL) {
			std::cerr << "Error:Memory allocation failed!" << std::endl;
			exit(MEMORY_ALLOCATION_ERROR);
		}
		tempParentNode->leftChild = new HuffmanTreeNode(nodes[0]);
		tempParentNode->rightChild = new HuffmanTreeNode(nodes[1]);

		nodes.erase(nodes.begin(),nodes.begin()+2);

		nodes.push_back(*tempParentNode);

		delete tempParentNode;
	}
}


void printHuffmanTree(HuffmanTreeNode *&node) {
	if (node != NULL) {
		if (node->leftChild != NULL) {
			node->leftChild->code = node->code+ '0';
			printHuffmanTree(node->leftChild);
		}
		if (node->rightChild != NULL) {
			node->rightChild->code += node->code+'1';
			printHuffmanTree(node->rightChild);
		}
		if(node->leftChild==NULL&&node->rightChild==NULL) {
			std::cout << *node << std::endl;
		}
	}

}


int inputLogicalValue(char FalseValue = '0', char TrueValue = '1')
{
	while (true) {
		char InputChar = _getch();
		if (InputChar == 0 || InputChar == -32)
			InputChar = _getch();
		else if (InputChar == FalseValue || InputChar == TrueValue) {
			std::cout << InputChar;
			return InputChar == TrueValue;
		}
	}
}


int inputInteger(int minNum, int maxNum, const char* prompt = "") {
	double num;
	while (true) {
		if (prompt != "")
			std::cout << "请输入" << prompt << "[范围" << minNum << "-" << maxNum << "]:";
		std::cin >> num;
		if (std::cin.fail() || static_cast<int>(num) != num || num < minNum || num > maxNum) {
			std::cout << "输入非法,请重新输入!" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return static_cast<int>(num);
		}
	}
}


int main() {
	do {
		system("cls");
		std::cout << "+-----------------------------+" << std::endl;
		std::cout << "|         最优二元树          |" << std::endl;
		std::cout << "|     Optimal Binary Tree     |" << std::endl;
		std::cout << "+-----------------------------+" << std::endl;

		int nodeNum = inputInteger(2, INT_MAX, ">>> 最优二元树节点个数");

		std::vector<HuffmanTreeNode> nodes(nodeNum,0);
		std::cout << std::endl;
		std::cout << ">>> 请输入最优二元树的节点" << std::endl;
		for (int i = 0; i < nodeNum; i++) {
			char temp[CHAR_MAX];
			sprintf(temp, "最优二元树的第 %d 个节点", i + 1);
			nodes[i].num = inputInteger(0, INT_MAX, temp);
		}

		createHuffmanTree(nodes);
		std::cout << std::endl;

		std::cout << ">>> 最优二元树编码:" << std::endl;
		HuffmanTreeNode* root = &nodes[0];
		printHuffmanTree(root);

		std::cout << "是否退出程序?[y/n]:";
	} while (!inputLogicalValue('n', 'y'));
	return 0;
}

