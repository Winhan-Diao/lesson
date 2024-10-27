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

int Pos(char ch, const char* str)// 返回指定字符ch在字符串str中的下标。不存在时返回-1
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
		if (' ' < (options[i] & 0x7f) && (options[i] & 0x7f) < 127)//选项options中的可打印字符
			cout << options[i] << ' ';
		else//选项options中的部分控制字符
		{
			switch (options[i])
			{
			case '\t':   cout << "[Tab] ";   break;//选项中的Tab字符'\t'（即ASCII编码9）
			case '\x1b': cout << "[Esc] ";   break;//选项中的Esc字符'\x1b'（即ASCII编码27）
			case ' ':    cout << "[Space] "; break;//选项中的空格字符' '（即ASCII编码32）
			}
		}
	}
	cout << "\b}: ";// '\b'退格,先退一格，在输出右花括号
	do
	{
		key = getchar();
	} while (options[0] != '\0' && Pos(key, options) < 0);//若options为空长度为0的字符串，则输入无限制；否则输入项应该在optins中
	system("cls");
	return key;
}
void test() 
{
	while (1)
	{
		cout << "输入任意键进行测试" << endl;
		cout << "1 --- 默认构造函数测试" << endl;
		cout << "2 --- 有参构造函数测试" << endl;
		cout << "3 --- 拷贝构造函数测试" << endl;
		cout << "4 --- 矩阵基本运算测试" << endl;
		cout << "5 --- 求矩阵行列式测试" << endl;
		cout << "6 --- 求矩阵逆矩阵测试" << endl;
		cout << "Esc键 --- 退出测试" << endl;
		int key = Choice("\n请选择", "1234567\x1b");
		if (key == 27)
		{
			break;
		}
		switch (key)
		{
		case '1':
		{
			cout << "1 --- 默认构造函数测试" << endl;
			test1();
			system("pause");
			system("cls");
			break;
		}
		case '2':
		{
			cout << "2 --- 有参构造函数测试" << endl;
			test2();
			system("pause");
			system("cls");
			break;
		}
		case '3':
		{
			cout << "3 --- 拷贝构造函数测试" << endl;
			test3();
			system("pause");
			system("cls");
			break;
		}
		case '4':
		{
			cout << "4 --- 矩阵基本运算测试" << endl;
			test4();
			system("pause");
			system("cls");
			break;
		}
		case '5':
		{
			cout << "5 --- 求矩阵行列式测试" << endl;
			test5();
			system("pause");
			system("cls");
			break;
		}
		case '6':
		{
			cout << "6 ---求矩阵逆矩阵测试" << endl;
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

void test1()//1 --- 默认构造函数测试
{
	Matrix<int>A;
	cout << A;
	
}

void test2()//2 --- 有参构造函数测试
{
	int m = 0;
	int n = 0;
	cout << "请输入矩阵行数 ：";
	cin >> m;
	getchar();
	cout << "请输入矩阵列数 ：";
	cin >> n;
	getchar();
	Matrix<int>A = Matrix<int>(m, n);
	cout << A;
}

void test3()// 3 -- - 拷贝构造函数测试
{
	int m = 0;
	int n = 0;
	cout << "请输入矩阵行数 ：";
	cin >> m;
	getchar();
	cout << "请输入矩阵列数 ：";
	cin >> n;
	getchar();
	Matrix<int>A = Matrix<int>(m, n);
	cout << "原矩阵为：" << endl;
	cout << A;
	Matrix<int>C(A);
	cout << "拷贝后矩阵为：" << endl;
	cout << C;
}

void test4()//4 --- 矩阵基本运算测试
{
	int m = 0;
	int n = 0;
	cout << "请输入矩阵A的行数 ：";
	cin >> m;
	getchar();
	cout << "请输入矩阵A的列数 ：";
	cin >> n;
	getchar();
	Matrix<int>A = Matrix<int>(m, n);
	int p = 0;
	int q = 0;
	cout << "请输入矩阵B的行数 ：";
	cin >> p;
	getchar();
	cout << "请输入矩阵B的列数 ：";
	cin >> q;
	getchar();
	Matrix<int>B = Matrix<int>(p, q);
	int a = 0;
	int b = 0;
	cout << "请输入矩阵C的行数 ：";
	cin >> a;
	getchar();
	cout << "请输入矩阵C的列数 ：";
	cin >> b;
	getchar();
	Matrix<int>C = Matrix<int>(a, b);
	cout << "矩阵A为：" << endl;
	cout << A;
	cout << "矩阵B为：" << endl;
	cout << B;
	cout << "矩阵C为：" << endl;
	cout << C;
	cout << "矩阵加法测试：A+B+C" << endl;
	cout << A + B + C;
	cout << "矩阵减法测试：A-B-C" << endl;
	cout << A - B - C;
	cout << "矩阵乘法测试：A*B*C" << endl;
	cout << A * B * C;
	cout << "负矩阵测试：" << endl;
	cout << "-A为:" << endl;
	cout << -A << endl;
	cout << "-B为:" << endl;
	cout << -B << endl;
	cout << "-C为:" << endl;
	cout << -C << endl;
}

void test5()//5 --- 求矩阵行列式测试
{
	int m = 0;
	int n = 0;
	cout << "请输入矩阵行数 ：";
	cin >> m;
	getchar();
	cout << "请输入矩阵列数 ：";
	cin >> n;
	getchar();
	Matrix<int>A = Matrix<int>(m, n);
	cout << "判断矩阵是否为零矩阵（是，返回1；否，返回0）:";
	cout << A.judge_zero_matrix() << endl;
	cout << "判断矩阵是否为方阵（是，返回1；否，返回0）:";
	cout << A.judge_square_matrix() << endl;
	cout << "原矩阵为：" << endl;
	cout << A;
	if (m == n)
	{
		cout << "原矩阵行列式为：" << A.determinant(A.matrix_ptr, A.n) << endl;
	}
	else
	{
		cout << "原矩阵无行列式！" << endl;
	}
}

void test6()//6 ---求矩阵逆矩阵测试
{
	int m = 0;
	int n = 0;
	cout << "请输入矩阵行数 ：";
	cin >> m;
	getchar();
	cout << "请输入矩阵列数 ：";
	cin >> n;
	getchar();
	Matrix<int>A = Matrix<int>(m, n);
	cout << "原矩阵为：" << endl;
	cout << A;
	double** inverse_matrix_ptr = new double* [n];
	for (int i = 0; i < n; i++)
	{
		inverse_matrix_ptr[i] = new double[n];
	}
	cout << "原矩阵的逆矩阵为：" << endl;
	A.inverse_matrix(A.matrix_ptr, inverse_matrix_ptr, A.n);
}