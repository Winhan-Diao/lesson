#include <iostream>
#include <vector>
#include <algorithm>
#include "abstract_vector.hpp"

using namespace std;

class Minterm {
public:
	int value;
	bool isCovered;
	Minterm() {
		value = 0;
		isCovered = false;
	}
};

class Implicant {
public:
	CollectionVector<Minterm*> minterms; // 使用AbstractVector替代vector
	CollectionVector<int> binary;       // 使用AbstractVector替代vector
	bool isUsed;
	Implicant() : isUsed(false) {}
	void print() const {
		for (size_t i = 0; i < binary.getSize(); i++) { // 使用size_t作为循环变量的类型
			if (binary[i] == 0) {
				cout << char('A' + i) << "'";
			}
			else if (binary[i] == 1) {
				cout << char('A' + i);
			}
			if (i < binary.getSize() - 1) { // 添加加号作为分隔符
			}
		}
		cout << endl;
	}
};
CollectionVector<int> getBinary(int minterm, int numVars) {
	CollectionVector<int> binary(numVars, 0);
	for (int i = numVars - 1; i >= 0; i--) {
		binary[i] = minterm % 2;
		minterm /= 2;
	}
	return binary;
}


bool isDifferByOne(CollectionVector<int> binary1, CollectionVector<int> binary2) {
	int count = 0;
	for (int i = 0; i < binary1.getSize(); i++) {
		if (binary1[i] != binary2[i]) {
			count++;
		}
		if (count > 1) {
			return false;
		}
	}
	return count == 1;
}

class QuineMcCluskey {

public:
	CollectionVector<Minterm> minterms;
	int numVars{};
	CollectionVector<CollectionVector<Implicant>> columns;
	CollectionVector<Implicant> essentialImplicants;
	CollectionVector<Implicant> primeImplicants;
	QuineMcCluskey() = default;

	void readData() {
		cout << "请输入变量数:";
		cin >> numVars;
		columns.resize(numVars + 1);
		cout << "请输入最小项的值 (输入-1结束):";
		int mintermValue;
		while (cin >> mintermValue) {
			if (mintermValue == -1) {
				break;
			}
			Minterm newMinterm;
			newMinterm.value = mintermValue;
			minterms.pushBack(newMinterm);
		}
	}

	static bool isInColumn( CollectionVector<Implicant>& column, const Implicant& implicant) {
		for (auto it = column.begin(); it != column.end(); ++it) {
			if (it->binary == implicant.binary) {
				return true;
			}
		}
	}

	void printResult()  {
		cout << endl << "最后化简结果: " << endl;
		auto it = essentialImplicants.begin();
		auto end = essentialImplicants.end();
		while (it != end) {
			it->print();
			++it;
		}
	}

	void initializeFirstColumnOfColumns() {
		for (auto& minterm : minterms) {
			Implicant newImplicant;
			newImplicant.minterms.pushBack(&minterm);
			newImplicant.binary = getBinary(minterm.value, numVars);
			columns[0].pushBack(newImplicant);
		}
	}

	void generateRestOfColumns() {
		for (int i = 0; i < numVars; i++) {
			for (int j = 0; j < columns[i].getSize(); j++) {
				for (int k = j + 1; k < columns[i].getSize(); k++) {
					Implicant& lastImplicant1 = columns[i][j];
					Implicant& lastImplicant2 = columns[i][k];

					if (isDifferByOne(lastImplicant1.binary,
						lastImplicant2.binary)) {

						lastImplicant1.isUsed = true;
						lastImplicant2.isUsed = true;
						Implicant newImplicant;
						newImplicant.binary = lastImplicant1.binary;
						for (int l = 0; l < newImplicant.binary.getSize(); l++) {
							if (newImplicant.binary[l] != lastImplicant2.binary[l]) {
								newImplicant.binary[l] = -1;
								break;
							}
						}
						newImplicant.minterms.insert(newImplicant.minterms.end(),
							lastImplicant1.minterms.begin(),
							lastImplicant1.minterms.end());
						newImplicant.minterms.insert(newImplicant.minterms.end(),
							lastImplicant2.minterms.begin(),
							lastImplicant2.minterms.end());
						if (!isInColumn(columns[i + 1], newImplicant)) {
							columns[i + 1].pushBack(newImplicant);
						}

					}
				}
			}
		}
	}

	void generatePrimeImplicants() {
		for (auto i = columns.begin(); i != columns.end(); ++i) {
			for (auto mintermIt = i->begin(); mintermIt != i->end(); ++mintermIt) {
				if (!mintermIt->isUsed) {
					primeImplicants.pushBack(*mintermIt);
				}
			}
		}
	}

	bool isMintermCoveredJustOnce(const Minterm& minterm) {
		int count = 0;
		for (auto primeImplicant : primeImplicants) {
			if (find(primeImplicant.minterms.begin(),
				primeImplicant.minterms.end(), &minterm)
				!= primeImplicant.minterms.end()) {
				count++;
			}
		}
		return count == 1;
	}

	void initializeEssentialPrimeImplicants() {
		for (auto& minterm : minterms) {
			if (isMintermCoveredJustOnce(minterm)) {
				for (auto& primeImplicant : primeImplicants) {
					if (find(primeImplicant.minterms.begin(),
						primeImplicant.minterms.end(), &minterm)
						!= primeImplicant.minterms.end()) {
						if (!primeImplicant.isUsed) {
							primeImplicant.isUsed = true;
							essentialImplicants.pushBack(primeImplicant);
						}
						break;
					}
				}
			}
		}
	}


	void removeCoveredMinterms() {
		for (auto& essentialImplicant : essentialImplicants) {
			for (auto minterm : essentialImplicant.minterms) {
				minterm->isCovered = true;
			}
		}
	}

	void extractEssentialPrimeImplicants() {

		initializeEssentialPrimeImplicants();
		while (true) {
			removeCoveredMinterms();
			int MaxNumCoveredMinterms = 0;
			Implicant* newEssentialImplicant = nullptr;
			for (auto& primeImplicant : primeImplicants) {
				if (!primeImplicant.isUsed) {
					int numCoveredMinterms = 0;
					for (auto minterm : primeImplicant.minterms) {
						if (!minterm->isCovered) {
							numCoveredMinterms++;
						}
					}
					if (numCoveredMinterms > MaxNumCoveredMinterms) {
						MaxNumCoveredMinterms = numCoveredMinterms;
						newEssentialImplicant = &primeImplicant;
					}
				}
			}
			if (!MaxNumCoveredMinterms) {
				break;
			}
			newEssentialImplicant->isUsed = true;
			essentialImplicants.pushBack(*newEssentialImplicant);
		}
	}


	void runAlgorithm() {

		readData();
		initializeFirstColumnOfColumns();
		generateRestOfColumns();
		generatePrimeImplicants();
		extractEssentialPrimeImplicants();
		printResult();
	}
};

int main() {
	QuineMcCluskey quineMcCluskey;
	quineMcCluskey.runAlgorithm();
	return 0;
}