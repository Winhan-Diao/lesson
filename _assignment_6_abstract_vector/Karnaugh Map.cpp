#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//第一步建立一个最小项类
class Minterm
{
public:
	int data;
	bool isCoverd; //判断是否已经被覆盖了
	Minterm() {
		data = 0;
		isCoverd = false;
	}
};
//第二步建立一个质蕴涵项类
class Implicant
{
public:
	vector<Minterm*>minterms;//我们后续需要处理的对象
	vector<int>binary;//仿造卡诺图，用二进制编码便于处理
	bool isUsed;
	Implicant() {
		isUsed = false;
	}
	void print() const{
		for (int i = 0; i < binary.size(); i++) {
			if (binary[i] == 0) cout << char('A' + i) << "'";  //A'代表A反
			else if (binary[i] == 1) cout << char('A' + i);
			if (i < binary.size())cout << "+";
		}
	}
};
//将最小项编号转换为二进制编码
vector<int>getBinary(int minterm, int digits) {
	vector<int>binary(digits, 0);
	for (int i = digits; i > 0; i--) {
		binary[i] = minterm % 2;
		minterm /= 2;
	}
	return binary;
}
//查询每一个最小项中1的个数，便于后续分组
int numbers(const vector<int>& binary) {
	int count = 0;
	for (int i = 0; i < binary.size(); i++) {
		if (binary[i] == 1)count++;
	}
	return count;
}
//化简过程，判断两个二进制编码间是否只有一项不同（两个“1”是否相邻）
bool isDifferByOne(vector<int>binary1, vector<int>binary2) {
	int count = 0;
	for (int i = 0; i < binary1.size(); i++) {
		if (binary1[i] != binary2[i])count++;
		if (count > 1)return false;
	}
	
	return count == 1;
}
//QM算法实现
class QM
{
public:
	vector<Minterm>minterms;
	int digits = 0;
	vector<Implicant>primeImplicants;
	vector<Implicant>essentialImplicants;
	vector<vector<Implicant>>columns;//存储分组结果
	QM() = default;
	void addData();
	void initializeColumn();
	void simplifyColumns();
	bool isInColumns(const vector<Implicant>& column,const Implicant& Implicant);
	void generatePrimeImplicant();
	bool isCoverdJustOnce(const Minterm& minterm);
	void generateEssentialPrimeImplicant();
	void removeCoverdMinterms();
	void extractEssentialPrimeImplicant();
	void result();
};
void QM::addData() {
	cout << "请输入逻辑函数的变量数：" << endl;
	cin >> digits;
	columns.resize(digits + 1);//还有全是0的一列
	cout << "请输入逻辑函数的最小项（以-1结尾）：" << endl;
	int mintermsData = 0;
	while (cin >> mintermsData) {
		if (mintermsData == -1)break;
		Minterm newMinterms;
		newMinterms.data = mintermsData;
		minterms.push_back(newMinterms);
	}
}
//使用迭代器遍历每一个最小项,以二进制编码形式存入column中，初始化column第一列
void QM::initializeColumn() {
	for (auto& minterm : minterms) {
		Implicant newImplicant;
		newImplicant.minterms.push_back(&minterm);
		newImplicant.binary = getBinary(minterm.data, digits);
		columns[0].push_back(newImplicant);
	}
}
//判断化简后的结果是否已经存在于columns中
bool QM::isInColumns(const vector<Implicant>& column, const Implicant& Implicant) {
	for (auto& primeImplicant : column) {
		if (primeImplicant.binary == Implicant.binary)return true;
	}
	return false;
}
//卡诺圈式化简
void QM::simplifyColumns() {
	for (int i = 0; i < digits; i++) {
		for (int j = 0; j < columns[i].size(); j++) {
			for (int k = j + 1; k < columns[i].size(); k++) {
				Implicant& lastImplicant1 = columns[i][j];
				Implicant& lastImplicant2 = columns[i][k];
				if (isDifferByOne(lastImplicant1.binary, lastImplicant2.binary)) {
					lastImplicant1.isUsed = true;
					lastImplicant2.isUsed = true;
					Implicant newImpliant;
					newImpliant.binary = lastImplicant1.binary;
					for (int i = 0; i < newImpliant.binary.size(); i++) {
						if (newImpliant.binary[i] != lastImplicant2.binary[i]) {
							newImpliant.binary[i] = -1;
							break;
						}
					}
					newImpliant.minterms.insert(newImpliant.minterms.begin(), lastImplicant1.minterms.begin(), lastImplicant1.minterms.end());
					newImpliant.minterms.insert(newImpliant.minterms.begin(), lastImplicant1.minterms.begin(), lastImplicant1.minterms.end());
					if (!isInColumns(columns[i + 1], newImpliant)) columns[i + 1].push_back(newImpliant);
				}
			}
		}
	}
}
void QM::generatePrimeImplicant() {
	for (auto& column : columns) {
		for (auto& minterm : column) {
			if (!minterm.isUsed) {
				minterm.isUsed = true;
				primeImplicants.push_back(minterm);
			}
		}
	}
}
bool QM::isCoverdJustOnce(const Minterm& minterm) {
	int count = 0;
	for (auto& primeImplicant : primeImplicants) {
		//判断每个最小项是否只被圈过一次，如果是，那就只有两种情况，单个“1”或是单个圈，用于后续找到必要质蕴涵项
		if (find(primeImplicant.minterms.begin(), primeImplicant.minterms.end(), &minterm) != primeImplicant.minterms.end())count++;
	}
	return count==1;
}
void QM::generateEssentialPrimeImplicant() {
	for (auto& minterm : minterms) {
		if (isCoverdJustOnce(minterm)) {
			for (auto& primeImplicant : primeImplicants) {
				if (find(primeImplicant.minterms.begin(), primeImplicant.minterms.end(), &minterm) != primeImplicant.minterms.end()) {
					if (!primeImplicant.isUsed) {
						primeImplicant.isUsed = true;
						essentialImplicants.push_back(primeImplicant);
					}
					break;
				}
			}
		}
	}
}
void QM::removeCoverdMinterms() {
	for (auto& essentialImplicant : essentialImplicants) {
		for (auto& minterm : essentialImplicant.minterms) {
			minterm->isCoverd = true;
		}
	}
}
void QM::extractEssentialPrimeImplicant() {
	generateEssentialPrimeImplicant();
	while (true) {
		removeCoverdMinterms();
		int maxNumber = 0;
		Implicant* newImplicant = nullptr;
		for (auto& primeImplicant : primeImplicants) {
			if (!primeImplicant.isUsed) {
				int minNumber = 0;
				for (auto& minterm : primeImplicant.minterms) {
					if (!minterm->isCoverd) minNumber++;
				}
				if (minNumber > maxNumber){
					maxNumber=minNumber;
					newImplicant = &primeImplicant;
					}
			}
		}
		if (!maxNumber)break;
		newImplicant->isUsed = true;
		essentialImplicants.push_back(*newImplicant);
	}
}
void QM::result() {

}