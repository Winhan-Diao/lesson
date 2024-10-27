#pragma once
#include<iostream>
#include<cstdlib>
#include<iomanip>
#include"MathVector.hpp"
using std::cout;
using std::endl;
using std::cin;
using std::setw;

template<class T>
class Matrix
{
//private:
public:
	int m=0;//矩阵行数（行向量个数）
	int n=0;//矩阵列数（行向量分量个数）
	MathVector<T>* a= nullptr;
public:
	Matrix();//默认构造函数
	Matrix(const int& m, const int& n);//有参构造函数
	Matrix(const Matrix& copy);//拷贝构造函数
	~Matrix(){
		delete[] a;
	}
	MathVector<T>& operator[](int i)const{
		return a[i];
	}
	Matrix operator+(const Matrix& b)const;//矩阵加法（实现连加）
	Matrix operator-(const Matrix& b)const;//矩阵减法（实现连减）
	Matrix operator-()const;//负矩阵
	Matrix operator*(const Matrix& b)const;//矩阵乘法（实现连乘）

	void show_matrix()const;//展示矩阵
	bool judge_zero_matrix()const;//判断是否为零矩阵（是，返回true；否，返回false）
	bool judge_square_matrix()const;//判断是否为方阵（是，返回true；否，返回false）
	T determinant(MathVector<T>* a, int n)const;//求方阵对应行列式的值
	void adjugate_matrix(MathVector<T>* a, Matrix<T>& adjugate_matrix_ptr, int n);//求伴随矩阵,仅在求矩阵的逆矩阵时被调用
	Matrix<double> inverse_matrix();//求矩阵的逆矩阵
};
template<class T> std::ostream& operator<<(std::ostream& out, const Matrix<T>& b);//运算符<<重载

template<class T>
Matrix<T>::Matrix()//默认构造函数
{
	m = 0;
	n = 0;
}

template<class T>
Matrix<T>::Matrix(const int& r, const int& c):m(r),n(c)//有参构造函数
{
	a=new MathVector<T>[r];
	for(int i=0;i<r;i++) a[i].resize(c);
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& copy)//拷贝构造函数
{
	this->m = copy.m;
	this->n = copy.n;
	this->a =new MathVector<T>[this->m];
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			a[i][j] = copy[i][j];
		}
	}
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
				temp[i][j] = this->a[i][j] + b[i][j];
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
				temp[i][j] = this->a[i][j] - b[i][j];
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
			this->a[i][j] = -(this->a[i][j]);
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
		cout << "cannot times" << endl;
		return *this;
	}
	else
	{
		Matrix<T> temp(this->m,b.n);
		for (int i = 0; i < this->m; i++)
			for (int j = 0; j < b.n; j++)
			{
				for (int k = 0; k < this->n; k++)
				{
					temp[i][j] += this->a[i][k] * b[k][j];
				}
			}
		return temp;
	}
}

template<class T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& b)//运算符<<重载
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
			cout << setw(2)<<a[i][j] << " ";
		}
		cout << setw(2) << a[i][n - 1] << setw(2) << ")" << endl;
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
T Matrix<T>::determinant(MathVector<T>* a, int n)const//求方阵对应行列式的值
{
	if (n == 1)
	{
		return a[0][0];
	}
	else if (n == 2)//递归终点（若n>=3）
	{
		return a[0][0] * a[1][1] - a[0][1] * a[1][0];
	}
	else
	{
		T matrix_ptr_value = 0;
		for (int k = 0; k < n; k++)
		{
			MathVector<T>* matrix_ptr_temp = new MathVector<T>[n - 1];//行列式每一行第一个元素，构成一个列向量
			int i2 = 1;//根据第0行第j列展开，原本的行列式直接从第一行开始拷贝
			for (int i1 = 0; i1 < n - 1; i1++)//新创建一个动态二维数组，用于存储，上一次矩阵除去一行一列元素后，的所有数据
			{
				matrix_ptr_temp[i1] .resize(n-1) ;//行列式第一行
				int j2 = 0;
				for (int j1 = 0; j1 < n - 1; j1++)
				{
					if (j2 == k)
					{
						j2++;
					}//去除第j列
					matrix_ptr_temp[i1][j1] = a[i2][j2++];//赋值给新的二维数组
				}
				i2++;
			}
			matrix_ptr_value += ((k & 0x0001) ? (-1) : 1) * a[0][k] * determinant(matrix_ptr_temp, n - 1);//计算代数余子式与对应项的乘积,并进行函数迭代
			delete[] matrix_ptr_temp;
		}
		return matrix_ptr_value;
	}
}

template<class T>
void Matrix<T>::adjugate_matrix(MathVector<T>* a, Matrix<T>& adjugate_matrix_ptr, int n)//求伴随矩阵
{
	Matrix<T> matrix_for_algebraic_cofactor(n-1,n-1); //暂时存放代数余子式的矩阵
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
					matrix_for_algebraic_cofactor[i][j] = a[i1][j1++];
				}
				i1++;
			}//实现了转置
			adjugate_matrix_ptr[y][x] = ((x + y) & 0x0001 ? (-1) : 1) * determinant(matrix_for_algebraic_cofactor.a, n - 1);
		}
	}
}

template<class T>
Matrix<double> Matrix<T>::inverse_matrix()
{
	T determinant_value = determinant(a, n); 
	Matrix<double> inverse_matrix_ptr(n,n);
	if (int(determinant_value) == 0)
	{
		cout << "No Inverse_matrix" << endl;
		return inverse_matrix_ptr;
	}
	else 
	{
		if (n == 1)
		{
			cout << "(" << setw(2) << 1.0 / a[0][0] << setw(2) << ")";
			return inverse_matrix_ptr;
		}
		Matrix<T> adjugate_matrix_ptr(n,n);
		for (int i = 0; i < n; i++)
		{
			adjugate_matrix_ptr[i].resize(n);
		}
		adjugate_matrix(a, adjugate_matrix_ptr, n);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				inverse_matrix_ptr[i][j] = adjugate_matrix_ptr[i][j] *1.0/ determinant_value;
			}
		}
		return inverse_matrix_ptr;
	}
}