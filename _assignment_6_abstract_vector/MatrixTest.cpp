#include<iostream>
#include"Matrix.hpp"
using namespace std;

void test();
int Pos(char ch, const char* str);
int Choice(const char* prompt, const char* options);
void test1();
void test2();
void test3();
void test4();
void test5();
void test6();

int main()
{
	test();
	system("pause");
	return 0;
}

int Pos(char ch, const char* str)// ����ָ���ַ�ch���ַ���str�е��±ꡣ������ʱ����-1
{
	int i;
	for (i = 0; str[i] != '\0'; i++)
		if (ch == str[i])
			return i;
	return -1;
}

int Choice(const char* prompt, const char* options)
{
	int key;
	cout << prompt << "{";
	for (int i = 0; options[i] != '\0'; i++)
	{
		if (' ' < (options[i] & 0x7f) && (options[i] & 0x7f) < 127)//ѡ��options�еĿɴ�ӡ�ַ�
			cout << options[i] << ' ';
		else//ѡ��options�еĲ��ֿ����ַ�
		{
			switch (options[i])
			{
			case '\t':   cout << "[Tab] ";   break;//ѡ���е�Tab�ַ�'\t'����ASCII����9��
			case '\x1b': cout << "[Esc] ";   break;//ѡ���е�Esc�ַ�'\x1b'����ASCII����27��
			case ' ':    cout << "[Space] "; break;//ѡ���еĿո��ַ�' '����ASCII����32��
			}
		}
	}
	cout << "\b}: ";// '\b'�˸�,����һ��������һ�����
	do
	{
		key = getchar();
	} while (options[0] != '\0' && Pos(key, options) < 0);//��optionsΪ�ճ���Ϊ0���ַ����������������ƣ�����������Ӧ����optins��
	system("cls");
	return key;
}
void test() 
{
	while (1)
	{
		cout << "������������в���" << endl;
		cout << "1 --- Ĭ�Ϲ��캯������" << endl;
		cout << "2 --- �вι��캯������" << endl;
		cout << "3 --- �������캯������" << endl;
		cout << "4 --- ��������������" << endl;
		cout << "5 --- ���������ʽ����" << endl;
		cout << "6 --- �������������" << endl;
		cout << "Esc�� --- �˳�����" << endl;
		int key = Choice("\n��ѡ��", "1234567\x1b");
		if (key == 27)
		{
			break;
		}
		switch (key)
		{
		case '1':
		{
			cout << "1 --- Ĭ�Ϲ��캯������" << endl;
			test1();
			system("pause");
			system("cls");
			break;
		}
		case '2':
		{
			cout << "2 --- �вι��캯������" << endl;
			test2();
			system("pause");
			system("cls");
			break;
		}
		case '3':
		{
			cout << "3 --- �������캯������" << endl;
			test3();
			system("pause");
			system("cls");
			break;
		}
		case '4':
		{
			cout << "4 --- ��������������" << endl;
			test4();
			system("pause");
			system("cls");
			break;
		}
		case '5':
		{
			cout << "5 --- ���������ʽ����" << endl;
			test5();
			system("pause");
			system("cls");
			break;
		}
		case '6':
		{
			cout << "6 ---�������������" << endl;
			test6();
			system("pause");
			system("cls");
			break;
		}
		default:
			break;
		}
	}
}

void test1()//1 --- Ĭ�Ϲ��캯������
{
	Matrix<int>A;
	cout << A;
	
}

void test2()//2 --- �вι��캯������
{
	int m = 0;
	int n = 0;
	cout << "������������� ��";
	cin >> m;
	getchar();
	cout << "������������� ��";
	cin >> n;
	getchar();
	Matrix<int>A = Matrix<int>(m, n);
	cout << A;
}

void test3()// 3 -- - �������캯������
{
	int m = 0;
	int n = 0;
	cout << "������������� ��";
	cin >> m;
	getchar();
	cout << "������������� ��";
	cin >> n;
	getchar();
	Matrix<int>A = Matrix<int>(m, n);
	cout << "ԭ����Ϊ��" << endl;
	cout << A;
	Matrix<int>C(A);
	cout << "���������Ϊ��" << endl;
	cout << C;
}

void test4()//4 --- ��������������
{
	int m = 0;
	int n = 0;
	cout << "���������A������ ��";
	cin >> m;
	getchar();
	cout << "���������A������ ��";
	cin >> n;
	getchar();
	Matrix<int>A = Matrix<int>(m, n);
	int p = 0;
	int q = 0;
	cout << "���������B������ ��";
	cin >> p;
	getchar();
	cout << "���������B������ ��";
	cin >> q;
	getchar();
	Matrix<int>B = Matrix<int>(p, q);
	int a = 0;
	int b = 0;
	cout << "���������C������ ��";
	cin >> a;
	getchar();
	cout << "���������C������ ��";
	cin >> b;
	getchar();
	Matrix<int>C = Matrix<int>(a, b);
	cout << "����AΪ��" << endl;
	cout << A;
	cout << "����BΪ��" << endl;
	cout << B;
	cout << "����CΪ��" << endl;
	cout << C;
	cout << "����ӷ����ԣ�A+B+C" << endl;
	cout << A + B + C;
	cout << "����������ԣ�A-B-C" << endl;
	cout << A - B - C;
	cout << "����˷����ԣ�A*B*C" << endl;
	cout << A * B * C;
	cout << "��������ԣ�" << endl;
	cout << "-AΪ:" << endl;
	cout << -A << endl;
	cout << "-BΪ:" << endl;
	cout << -B << endl;
	cout << "-CΪ:" << endl;
	cout << -C << endl;
}

void test5()//5 --- ���������ʽ����
{
	int m = 0;
	int n = 0;
	cout << "������������� ��";
	cin >> m;
	getchar();
	cout << "������������� ��";
	cin >> n;
	getchar();
	Matrix<int>A = Matrix<int>(m, n);
	cout << "�жϾ����Ƿ�Ϊ������ǣ�����1���񣬷���0��:";
	cout << A.judge_zero_matrix() << endl;
	cout << "�жϾ����Ƿ�Ϊ�����ǣ�����1���񣬷���0��:";
	cout << A.judge_square_matrix() << endl;
	cout << "ԭ����Ϊ��" << endl;
	cout << A;
	if (m == n)
	{
		cout << "ԭ��������ʽΪ��" << A.determinant(A.matrix_ptr, A.n) << endl;
	}
	else
	{
		cout << "ԭ����������ʽ��" << endl;
	}
}

void test6()//6 ---�������������
{
	int m = 0;
	int n = 0;
	cout << "������������� ��";
	cin >> m;
	getchar();
	cout << "������������� ��";
	cin >> n;
	getchar();
	Matrix<int>A = Matrix<int>(m, n);
	cout << "ԭ����Ϊ��" << endl;
	cout << A;
	double** inverse_matrix_ptr = new double* [n];
	for (int i = 0; i < n; i++)
	{
		inverse_matrix_ptr[i] = new double[n];
	}
	cout << "ԭ����������Ϊ��" << endl;
	A.inverse_matrix(A.matrix_ptr, inverse_matrix_ptr, A.n);
}