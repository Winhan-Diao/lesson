
#include <iostream>
#include <algorithm>
#include "abstract_vector.hpp"

#pragma once
#include <cstring>
#include <memory>
#include <type_traits>
#include <cmath>
#include "c_alloc_allocator.hpp"
using namespace std;
template <class T, class Alloc = std::allocator<T>>
//第一步建立一个最小项类
class Minterm:public AbstractVector<T, Alloc>
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
template <class T, class Alloc = std::allocator<T>>
class Implicant:public AbstractVector<T,Alloc>
{
public:
	std::unique_ptr<AbstractVector<Minterm<T, Alloc>, Alloc>*>minterms;//我们后续需要处理的对象
	std::unique_ptr<AbstractVector<int>> binary;//仿造卡诺图，用二进制编码便于处理
	bool isUsed;
	Implicant() {
		isUsed = false;
	}
	void print() const {
		for (size_t i = 0; i < binary.size(); i++) {
			if (binary[i] == 0) cout << char('A' + i) << "'"; // A'代表A反
			else if (binary[i] == 1) cout << char('A' + i);
			if (i < binary.size() - 1) cout << "+"; // 避免最后一个元素后输出 "+"
		}
		cout << endl; // 输出完后换行
	}
	AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>& other)override {
		cout << endl;
		return *this;
	};
	AbstractVector<T, Alloc>& operator<<(long long) override {
		cout << endl;
		return *this;
	}
};
//将最小项编号转换为二进制编码
//static std::unique_ptr<AbstractVector<int, AbstractVector<int>::allocator_type>> getBinary(int minterm, int digits) {
//	AbstractVector<int>binary;
//	for (int i = digits; i > 0; i--) {
//		binary.set(i,minterm);
//		minterm /= 2;
//	}
//	return std::make_unique<Minterm<int, AbstractVector<int>::allocator_type>>(std::move(binary));
//}

//化简过程，判断两个二进制编码间是否只有一项不同（两个“1”是否相邻）
bool isDifferByOne(const AbstractVector<int>& binary1, const AbstractVector<int>& binary2) {
	int count = 0;
	for (int i = 0; i < binary1.getSize(); i++) {
		if (binary1[i] != binary2[i])count++;
		if (count > 1)return false;
	}
	
	return count == 1;
}
//QM算法实现
template <class T, class Alloc = std::allocator<T>>
class QM :public AbstractVector<T, Alloc>
{
public:
	std::unique_ptr<AbstractVector<Minterm<T, Alloc>, Alloc>> minterms;
	int digits = 0;
	std::unique_ptr<AbstractVector<Implicant<T, Alloc>,Alloc>>primeImplicants;
	std::unique_ptr<AbstractVector<Implicant<T, Alloc>,Alloc>>essentialImplicants;
	std::unique_ptr<AbstractVector<AbstractVector<Implicant<T, Alloc>,Alloc>,Alloc>>columns;//存储分组结果
	QM() = default;
	void addData();
	void initializeColumn();
	void simplifyColumns();
	bool isInColumns(const AbstractVector<Implicant<T,Alloc>>& column, const Implicant<T, Alloc>& Implicant);
	void generatePrimeImplicant();
	bool isCoverdJustOnce(const Minterm<T,Alloc>& minterm);
	void generateEssentialPrimeImplicant();
	void removeCoverdMinterms();
	void extractEssentialPrimeImplicant();
	void result();
	void run();
	AbstractVector <int> getbinary(int minterm, int digits) {
		AbstractVector<int>binary;
		for (int i = digits; i > 0; i--) {
			binary.set(i, minterm%2);
			minterm /= 2;
		}
		return binary;
	};
	AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>& other)override {
		cout << endl;
		return *this;
	};
	AbstractVector<T, Alloc>& operator<<(long long)  override{ 
		cout << endl; 
		return *this;
	}
};
//template <class T, class Alloc>
//AbstractVector <int>QM<T,Alloc>::getbinary(int minterm, int digits) {
//	AbstractVector<int>binary;
//	for (int i = digits; i > 0; i--) {
//		binary.set(i,minterm);
//		minterm /= 2;
//	}
//	return binary;
//}
template <class T, class Alloc>
void QM<T, Alloc>::addData(){
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
template <class T, class Alloc>
void QM<T,Alloc>::initializeColumn() {
	for (auto& minterm : minterms) {
		Implicant newImplicant;
		newImplicant.minterms.push_back(&minterm);
		newImplicant.binary = getBinary(minterm.data, digits);
		columns[0].push_back(newImplicant);
	}
}
//判断化简后的结果是否已经存在于columns中
template <class T, class Alloc>
bool QM<T,Alloc>::isInColumns(const AbstractVector<Implicant<T, Alloc>>& column, const Implicant<T, Alloc>& Implicant) {
	for (auto& primeImplicant : column) {
		if (primeImplicant.binary == Implicant.binary)return true;
	}
	return false;
}
//卡诺圈式化简
template <class T, class Alloc>
void QM<T, Alloc>::simplifyColumns() {
	for (int i = 0; i < digits; i++) {
		for (int j = 0; j < columns[i].size(); j++) {
			for (int k = j + 1; k < columns[i].size(); k++) {
				Implicant<T, Alloc>& lastImplicant1 = columns[i][j];
				Implicant<T, Alloc>& lastImplicant2 = columns[i][k];
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
template <class T, class Alloc>
void QM<T,Alloc>::generatePrimeImplicant() {
	for (auto& column : columns) {
		for (auto& minterm : column) {
			if (!minterm.isUsed) {
				minterm.isUsed = true;
				primeImplicants.push_back(minterm);
			}
		}
	}
}
template <class T, class Alloc>
bool QM<T,Alloc>::isCoverdJustOnce(const Minterm<T,Alloc>& minterm) {
	int count = 0;
	for (auto& primeImplicant : primeImplicants) {
		//判断每个最小项是否只被圈过一次，如果是，那就只有两种情况，单个“1”或是单个圈，用于后续找到必要质蕴涵项
		if (find(primeImplicant.minterms.begin(), primeImplicant.minterms.end(), &minterm) != primeImplicant.minterms.end())count++;
	}
	return count==1;
}
template <class T, class Alloc>
void QM<T, Alloc>::generateEssentialPrimeImplicant() {
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
template <class T, class Alloc>
void QM<T, Alloc>::removeCoverdMinterms() {
	for (auto& essentialImplicant : essentialImplicants) {
		for (auto& minterm : essentialImplicant.minterms) {
			minterm->isCoverd = true;
		}
	}
}
template <class T, class Alloc>
void QM<T, Alloc>::extractEssentialPrimeImplicant() {
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
template <class T, class Alloc>
void QM<T, Alloc>::result() {
	cout << "初化简结果为：" << endl;
	for (auto& primeImplicant : primeImplicants) {
		primeImplicant.print();
	}
	cout << "最终化简结果为：" << endl;
	for (auto& essentialImplicant : essentialImplicants) {
		essentialImplicant.print();
	}
}
template <class T, class Alloc>
void QM<T, Alloc>::run() {
	addData();
	initializeColumn();
	simplifyColumns();
	generatePrimeImplicant();
	extractEssentialPrimeImplicant();
	result();
}
int main() {
	QM<int> qm;
	qm.run();
}