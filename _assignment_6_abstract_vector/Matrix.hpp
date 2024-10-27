#pragma
#include<iostream>
#include<cstdlib>
#include<iomanip>
//#include"Vector.h"
using std::cout;
using std::endl;
using std::cin;
using std::setw;

template<class T>
class Matrix//:public Vector
{
//private:
public:
	int m=0;//矩阵行数（行向量个数）
	int n=0;//矩阵列数（行向量分量个数）
	T** matrix_ptr = nullptr;
public:
	Matrix();//默认构造函数
	Matrix(const int& m, const int& n);//有参构造函数
	Matrix(const Matrix& copy);//拷贝构造函数
	~Matrix();//析构函数

	Matrix operator+(const Matrix& b)const;//矩阵加法（实现连加）
	Matrix operator-(const Matrix& b)const;//矩阵减法（实现连减）
	Matrix operator-()const;//负矩阵
	Matrix operator*(const Matrix& b)const;//矩阵乘法（实现连乘）

	void show_matrix()const;//展示矩阵
	bool judge_zero_matrix()const;//判断是否为零矩阵（是，返回true；否，返回false）
	bool judge_square_matrix()const;//判断是否为方阵（是，返回true；否，返回false）
	T determinant(T** matrix_ptr, int n)const;//求方阵对应行列式的值
	void adjugate_matrix(T** matrix_ptr, T** adjugate_matrix_ptr, int n);//求伴随矩阵,仅在求矩阵的逆矩阵时被调用
	void inverse_matrix(T** matrix_ptr, double** inverse_matrix_ptr, int n);//求矩阵的逆矩阵
};
template<class T> std::ostream& operator<<(std::ostream& out, const Matrix<T>& b);//运算符<<重载

template<class T>
Matrix<T>::Matrix()//默认构造函数
{
	m = 0;
	n = 0;
	matrix_ptr = new  T*;
	matrix_ptr[m] = new T[n];
	matrix_ptr[m][n] = 0;
}

template<class T>
Matrix<T>::Matrix(const int& m, const int& n)//有参构造函数
{
	//以n作为行向量分量个数，通过调用向量类中的随机生成向量函数，
	// 随机创建m个类似向量，并将数据依次传入matrix二维数组中
	this->m = m;
	this->n = n;
	matrix_ptr = new T * [m];
	for (int i = 0; i < m; i++)
	{
		matrix_ptr[i] = new T[n];
	}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			matrix_ptr[i][j] = rand() % 10 + 1;
		}
	}
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& copy)//拷贝构造函数
{
	this->m = copy.m;
	this->n = copy.n;
	this->matrix_ptr = new T * [m];
	for (int i = 0; i < m; i++)
	{
		matrix_ptr[i] = new T[n];
	}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			matrix_ptr[i][j] = copy.matrix_ptr[i][j];
		}
	}
}

template<class T>
Matrix<T>::~Matrix()//析构函数
{
	for (int i = 0; i < m; i++)
	{
		delete[] matrix_ptr[i];
	}
	delete[] matrix_ptr;
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& b)const//矩阵加法（实现连加）
{
	if (this->judge_zero_matrix())
	{
		return b;
	}
	else if (b.judge_zero_matrix())
	{
		return *this;
	}
	else
	{
		Matrix<T> temp(b);
		for (int i=0; i < m; i++)
			for (int j=0; j < n; j++)
			{
				temp.matrix_ptr[i][j] = this->matrix_ptr[i][j] + b.matrix_ptr[i][j];
			}
		return temp;
	}
}

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& b)const//矩阵减法（实现连减）
{
	if (this->judge_zero_matrix())
	{
		return -b;//调用operator-的重载函数
	}
	else if (b.judge_zero_matrix())
	{
		return *this;
	}
	else
	{
		Matrix<T> temp(b);
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
			{
				temp.matrix_ptr[i][j] = this->matrix_ptr[i][j] - b.matrix_ptr[i][j];
			}
		return temp;
	}
}

template<class T>
Matrix<T> Matrix<T>::operator-()const//负矩阵
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			this->matrix_ptr[i][j] = -(this->matrix_ptr[i][j]);
		}
	return *this;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix& b)const//矩阵乘法（实现连乘）
{
	if (this->judge_zero_matrix())
	{
		Matrix<T> a;
		return a;
	}
	else if (b.judge_zero_matrix())
	{
		Matrix<T> a;
		return a;
	}
	else if (this->n != b.m)
	{
		cout << "不符合矩阵乘法定义！" << endl;
		return *this;
	}
	else
	{
		int index = 0;
		T* multiply = new T[(this->m) * (b.n)];
		for (int i = 0; i < (this->m) * (b.n); i++)
		{
			multiply[i] = 0;
		}
		Matrix<T> temp(this->m, b.n);
		for (int i = 0; i < temp.m; i++)
		{
			for (int j = 0; j < temp.n; j++)
			{
				for (int k = 0; k < this->n; k++)
				{
					multiply[index] += this->matrix_ptr[i][k] * b.matrix_ptr[k][j];
				}
				index++;
			}
		}
		int time = 0;
		for (int i = 0; i < temp.m; i++)
		{
			for (int j = 0; j < temp.n; j++)
			{
				temp.matrix_ptr[i][j] = multiply[j + time * temp.n];
			}
			time++;
		}
		return temp;
	}
}

template<class T>
std::ostream& operator<<(std::ostream& out, const typename Matrix<T>& b)//运算符<<重载
{
	b.show_matrix();
	return out;
}

template<class T>
void Matrix<T>::show_matrix()const//展示矩阵
{
	if (m == 0 || n == 0)
	{
		cout << "(" << setw(2) << 0 << setw(2) << ")" << endl;
		cout << endl;
		return;
	}
	for (int i = 0; i < m; i++)
	{
		cout << "(";
		for (int j = 0; j < n - 1; j++)
		{
			cout << setw(2)<<matrix_ptr[i][j] << " ";
		}
		cout << setw(2) << matrix_ptr[i][n - 1] << setw(2) << ")" << endl;
	}
	cout << endl;
}

template<class T>
bool Matrix<T>::judge_zero_matrix()const//判断是否为零矩阵（是，返回true；否，返回false）
{
	if (this->m == 0 || this->n == 0)
		return true;
	else
		return false;
}

template<class T>
bool Matrix<T>::judge_square_matrix()const//判断是否为方阵（是，返回true；否，返回false）
{
	if (this->m == this->n)
		return true;
	else
		return false;
}

template<class T>
T Matrix<T>::determinant(T** matrix_ptr, int n)const//求方阵对应行列式的值
{
	if (n == 1)
	{
		return matrix_ptr[0][0];
	}
	else if (n == 2)//递归终点（若n>=3）
	{
		return matrix_ptr[0][0] * matrix_ptr[1][1] - matrix_ptr[0][1] * matrix_ptr[1][0];
	}
	else
	{
		T matrix_ptr_value = 0;
		for (int k = 0; k < n; k++)
		{
			T** matrix_ptr_temp = new T * [n - 1];//行列式每一行第一个元素，构成一个列向量
			int i2 = 1;//根据第0行第j列展开，原本的行列式直接从第一行开始拷贝
			for (int i1 = 0; i1 < n - 1; i1++)//新创建一个动态二维数组，用于存储，上一次矩阵除去一行一列元素后，的所有数据
			{
				matrix_ptr_temp[i1] = new T[n - 1];//行列式第一行
				int j2 = 0;
				for (int j1 = 0; j1 < n - 1; j1++)
				{
					if (j2 == k)
					{
						j2++;
					}//去除第j列
					matrix_ptr_temp[i1][j1] = matrix_ptr[i2][j2++];//赋值给新的二维数组
				}
				i2++;
			}
			matrix_ptr_value += ((k & 0x0001) ? (-1) : 1) * matrix_ptr[0][k] * determinant(matrix_ptr_temp, n - 1);//计算代数余子式与对应项的乘积,并进行函数迭代
		}
		return matrix_ptr_value;
	}
}

template<class T>
void Matrix<T>::adjugate_matrix(T** matrix_ptr, T** adjugate_matrix_ptr, int n)//求伴随矩阵
{
	T** matrix_for_algebraic_cofactor = new T * [n - 1];//暂时存放代数余子式的矩阵
	for (int i = 0; i < n - 1; i++)
	{
		matrix_for_algebraic_cofactor[i] = new T[n - 1];//初始化分配空间
	}
	for (int x = 0; x < n; x++)
	{
		for (int y = 0; y < n; y++)
		{
			int i1 = 0;
			for (int i = 0; i < n - 1; i++)
			{
				if (i1 == x)
				{
					i1++;
				}
				int j1 = 0;
				for (int j = 0; j < n - 1; j++)
				{
					if (j1 == y)
					{
						j1++;
					}
					matrix_for_algebraic_cofactor[i][j] = matrix_ptr[i1][j1++];
				}
				i1++;
			}//实现了转置
			adjugate_matrix_ptr[y][x] = ((x + y) & 0x0001 ? (-1) : 1) * determinant(matrix_for_algebraic_cofactor, n - 1);
		}
	}
	for (int i = 0; i < n - 1; i++)
	{
		delete[] matrix_for_algebraic_cofactor[i];
	}
	delete[] matrix_for_algebraic_cofactor;
}

template<class T>
void Matrix<T>::inverse_matrix(T** matrix_ptr, double** inverse_matrix_ptr, int n)
{
	T determinant_value = determinant(matrix_ptr, n);
	if (int(determinant_value) == 0)
	{
		cout << "该矩阵不可逆!" << endl;
		return;
	}
	else 
	{
		if (n == 1)
		{
			cout << "(" << setw(2) << 1.0 / matrix_ptr[0][0] << setw(2) << ")";
			return;
		}
		T** adjugate_matrix_ptr = new T* [n];
		for (int i = 0; i < n; i++)
		{
			adjugate_matrix_ptr[i] = new T[n];
		}
		adjugate_matrix(matrix_ptr, adjugate_matrix_ptr, n);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				inverse_matrix_ptr[i][j] = adjugate_matrix_ptr[i][j] *1.0/ determinant_value;
			}
		}
		for (int i = 0; i < n; i++)
		{
			cout << "(";
			for (int j = 0; j < n - 1; j++)
			{
				cout << setw(10) << inverse_matrix_ptr[i][j] << " ";
			}
			cout << setw(10) << inverse_matrix_ptr[i][n - 1] << setw(2) << ")" << endl;
		}
		cout << endl;
		for (int i = 0; i < n; i++)
		{
			delete[] adjugate_matrix_ptr[i];
		}
		delete[] adjugate_matrix_ptr;
	}
}