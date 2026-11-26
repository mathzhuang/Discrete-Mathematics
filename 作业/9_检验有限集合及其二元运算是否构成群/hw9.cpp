#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <limits>
#include <algorithm>

using namespace std;

class GroupChecker {
private:
    vector<int> elements;
    vector<vector<int>> table;
    int n;
    int identityIndex = -1;

  
    int getElementIndex(int val) {
        for (int i = 0; i < n; ++i) {
            if (elements[i] == val) return i;
        }
        return -1;
    }

    bool isValidInteger(const string& s, int& result) {
        if (s.empty()) return false;

        size_t start = 0;
        if (s[0] == '-' || s[0] == '+') {
            if (s.length() == 1) return false; 
            start = 1;
        }

        for (size_t i = start; i < s.length(); ++i) {
            if (!isdigit(s[i])) {
                return false; 
            }
        }

        try {
            result = stoi(s);
        }
        catch (...) {
            return false;
        }
        return true;
    }

public:
    GroupChecker() : n(0) {}

    void inputSet() {
        cout << ">>> 步骤1: 输入集合信息 <<<" << endl;
        while (true) {
            cout << "请输入集合元素的个数 N (正整数): ";
            string input;
            cin >> input;
            int val;
            if (isValidInteger(input, val) && val > 0) {
                n = val;
                break;
            }
            cout << "[错误]: 输入无效。请输入一个纯正整数 (不含小数或字母)。" << endl;
        }

        cout << "请输入 " << n << " 个不重复的整数。" << endl;
        set<int> checkDup;
        elements.resize(n);

        int count = 0;
        while (count < n) {
            cout << "-> 请输入第 [" << count + 1 << "/" << n << "] 个元素: ";
            string input;
            cin >> input;

            int val;
            if (!isValidInteger(input, val)) {
                cout << "   [错误]: \"" << input << "\" 不是合法的整数 (包含乱码或小数)。请重新输入。" << endl;
                continue; 
            }

            if (checkDup.find(val) != checkDup.end()) {
                cout << "   [错误]: 元素 " << val << " 已存在于集合中。集合具有互异性。请重新输入。" << endl;
                continue;
            }

            elements[count] = val;
            checkDup.insert(val);
            count++;
            cout << "   [成功]: 元素 " << val << " 已记录。" << endl;
        }
    }

    bool inputAndCheckClosure() {
        cout << "\n>>> 步骤2: 输入二元运算表 (凯莱表) <<<" << endl;
        cout << "说明: 请依次输入运算结果。" << endl;

        table.assign(n, vector<int>(n));
        bool isClosed = true;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                bool validInput = false;
                while (!validInput) {
                    cout << "计算: " << elements[i] << " * " << elements[j] << " = ";
                    string input;
                    cin >> input;

                    int resultVal;
                    if (!isValidInteger(input, resultVal)) {
                        cout << "   [错误]: 输入包含非法字符或为浮点数，请输入整数。" << endl;
                        continue; 
                    }

                    validInput = true;
                    int idx = getElementIndex(resultVal);

                    if (idx == -1) {
                        cout << "   [警告]: 结果 " << resultVal << " 不在原集合中 (违反封闭性)。" << endl;
                        table[i][j] = -1;
                        isClosed = false;
                    }
                    else {
                        table[i][j] = idx;
                    }
                }
            }
        }

        if (!isClosed) {
            cout << "\n[判定结果]: 不构成群。" << endl;
            cout << "[原因]: 违反【封闭性】。" << endl;
        }
        return isClosed;
    }

    bool checkAssociativity() {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < n; ++k) {
                    int ab = table[i][j];
                    if (ab == -1) continue;
                    int left = table[ab][k];

                    int bc = table[j][k];
                    if (bc == -1) continue;
                    int right = table[i][bc];

                    if (left != right) {
                        cout << "\n[判定结果]: 不构成群。" << endl;
                        cout << "[原因]: 违反【结合律】 ("
                            << elements[i] << "*" << elements[j] << ")*" << elements[k] << " != "
                            << elements[i] << "*(" << elements[j] << "*" << elements[k] << ")" << endl;
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool checkIdentity() {
        for (int e = 0; e < n; ++e) {
            bool isIdentity = true;
            for (int x = 0; x < n; ++x) {
                if (table[e][x] != x || table[x][e] != x) {
                    isIdentity = false;
                    break;
                }
            }
            if (isIdentity) {
                identityIndex = e;
                cout << "[信息]: 找到单位元 e = " << elements[e] << endl;
                return true;
            }
        }
        cout << "\n[判定结果]: 不构成群。" << endl;
        cout << "[原因]: 违反【单位元】存在性。" << endl;
        return false;
    }

    bool checkInverse() {
        if (identityIndex == -1) return false;
        for (int i = 0; i < n; ++i) {
            bool hasInverse = false;
            for (int j = 0; j < n; ++j) {
                if (table[i][j] == identityIndex && table[j][i] == identityIndex) {
                    hasInverse = true;
                    break;
                }
            }
            if (!hasInverse) {
                cout << "\n[判定结果]: 不构成群。" << endl;
                cout << "[原因]: 违反【逆元】存在性。元素 " << elements[i] << " 无逆元。" << endl;
                return false;
            }
        }
        return true;
    }

    void run() {
        inputSet();
        if (!inputAndCheckClosure()) return;
        if (!checkAssociativity()) return;
        if (!checkIdentity()) return;
        if (!checkInverse()) return;

        cout << "\n*********************************" << endl;
        cout << " [判定结果]: 该集合关于给定运算构成一个群！" << endl;
        cout << "*********************************" << endl;
    }
};

int main() {
    GroupChecker checker;
    checker.run();
    return 0;
}