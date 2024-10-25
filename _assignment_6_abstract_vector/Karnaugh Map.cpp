#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//��һ������һ����С����
class Minterm
{
public:
	int data;
	bool isCoverd; //�ж��Ƿ��Ѿ���������
	Minterm() {
		data = 0;
		isCoverd = false;
	}
};
//�ڶ�������һ�����̺�����
class Implicant
{
public:
	vector<Minterm*>minterms;//���Ǻ�����Ҫ����Ķ���
	vector<int>binary;//���쿨ŵͼ���ö����Ʊ�����ڴ���
	bool isUsed;
	Implicant() {
		isUsed = false;
	}
	void print() const{
		for (int i = 0; i < binary.size(); i++) {
			if (binary[i] == 0) cout << char('A' + i) << "'";  //A'����A��
			else if (binary[i] == 1) cout << char('A' + i);
			if (i < binary.size())cout << "+";
		}
	}
};
//����С����ת��Ϊ�����Ʊ���
vector<int>getBinary(int minterm, int digits) {
	vector<int>binary(digits, 0);
	for (int i = digits; i > 0; i--) {
		binary[i] = minterm % 2;
		minterm /= 2;
	}
	return binary;
}
//��ѯÿһ����С����1�ĸ��������ں�������
int numbers(const vector<int>& binary) {
	int count = 0;
	for (int i = 0; i < binary.size(); i++) {
		if (binary[i] == 1)count++;
	}
	return count;
}
//������̣��ж����������Ʊ�����Ƿ�ֻ��һ�ͬ��������1���Ƿ����ڣ�
bool isDifferByOne(vector<int>binary1, vector<int>binary2) {
	int count = 0;
	for (int i = 0; i < binary1.size(); i++) {
		if (binary1[i] != binary2[i])count++;
		if (count > 1)return false;
	}
	
	return count == 1;
}
//QM�㷨ʵ��
class QM
{
public:
	vector<Minterm>minterms;
	int digits = 0;
	vector<Implicant>primeImplicants;
	vector<Implicant>essentialImplicants;
	vector<vector<Implicant>>columns;//�洢������
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
	cout << "�������߼������ı�������" << endl;
	cin >> digits;
	columns.resize(digits + 1);//����ȫ��0��һ��
	cout << "�������߼���������С���-1��β����" << endl;
	int mintermsData = 0;
	while (cin >> mintermsData) {
		if (mintermsData == -1)break;
		Minterm newMinterms;
		newMinterms.data = mintermsData;
		minterms.push_back(newMinterms);
	}
}
//ʹ�õ���������ÿһ����С��,�Զ����Ʊ�����ʽ����column�У���ʼ��column��һ��
void QM::initializeColumn() {
	for (auto& minterm : minterms) {
		Implicant newImplicant;
		newImplicant.minterms.push_back(&minterm);
		newImplicant.binary = getBinary(minterm.data, digits);
		columns[0].push_back(newImplicant);
	}
}
//�жϻ����Ľ���Ƿ��Ѿ�������columns��
bool QM::isInColumns(const vector<Implicant>& column, const Implicant& Implicant) {
	for (auto& primeImplicant : column) {
		if (primeImplicant.binary == Implicant.binary)return true;
	}
	return false;
}
//��ŵȦʽ����
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
		//�ж�ÿ����С���Ƿ�ֻ��Ȧ��һ�Σ�����ǣ��Ǿ�ֻ�����������������1�����ǵ���Ȧ�����ں����ҵ���Ҫ���̺���
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