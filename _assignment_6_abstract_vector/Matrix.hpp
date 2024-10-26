#ifndef MATRIX_HPP
#define MATRIX_HPP
#include<iostream>
//#include"Vector.h"
using std::cout;
using std::endl;
using std::cin;

template<class T>
class Matrix//:public Vector
{
private:
	int m;//����������������������
	int n;//��������������������������
	T** matrix_ptr = nullptr;
public:
	Matrix();//Ĭ�Ϲ��캯��
	Matrix(const int& m, const int& n);//�вι��캯��
	Matrix(const Matrix& copy);//�������캯��
	~Matrix();//��������

	Matrix operator+(const Matrix& b)const;//����ӷ���ʵ�����ӣ�
	Matrix operator-(const Matrix& b)const;//���������ʵ��������
	Matrix operator-()const;//������
	Matrix operator*(const Matrix& b)const;//����˷���ʵ�����ˣ�
	friend std::ostream& operator<<(std::ostream& out, const Matrix& b);
	friend std::istream& operator>>(std::istream& in, const Matrix& b);

	void show_matrix()const;//չʾ����
	bool judge_zero_matrix()const;//�ж��Ƿ�Ϊ������ǣ�����true���񣬷���false��
	//Matrix inverse_matrix(T** matrix_ptr, T** inverse_matrix_ptr, int n)const;//�����������
	bool judge_square_matrix()const;//�ж��Ƿ�Ϊ�����ǣ�����true���񣬷���false��
	T determinant(T** matrix_ptr, int n)const;//�����Ӧ����ʽ��ֵ
	//Matrix transposed_matrix()const;//��ת�þ���
};

template<class T>
Matrix<T>::Matrix()
{
	this->m = 0;
	this->n = 0;
}

template<class T>
Matrix<T>::Matrix(const int& m, const int& n)
{
	//��n��Ϊ����������������ͨ�������������е������������������
	// �������m�����������������������δ���matrix��ά������
	matrix_ptr = new T * [m];
	for (int i = 0; i < m; i++)
	{
		matrix_ptr[i] = new T[n];
	}
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& copy)
{
	this->m = copy->m;
	this->n = copy->n;
	this->matrix_ptr = new T * [m];
	for (int i = 0; i < m; i++)
	{
		matrix_ptr[i] = new T[n];
	}
}

template<class T>
Matrix<T>::~Matrix()
{
	for (int i = 0; i < m; i++)
	{
		delete[] matrix_ptr[i];
	}
	delete[] matrix_ptr;
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& b)const
{
	if (this->judge_zero_vector())
	{
		b->show_matrix();
		return b;
	}
	else if (b->judge_zero_vector())
	{
		this->show_matrix();
		return *this;
	}
	else
	{
		Matrix<T> temp(b);
		int i = 0;
		int j = 0;
		for (; i < m; i++)
			for (; j < n; j++)
			{
				temp->matrix_ptr[i][j] = this->matrix_ptr[i][j] + b->matrix_ptr[i][j];
			}
		temp->show_matrix();
		return temp;
	}
}

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& b)const
{
	if (this->judge_zero_vector())
	{
		return -b;//����operator-�����غ���
	}
	else if (b->judge_zero_vector())
	{
		this->show_matrix();
		return *this;
	}
	else
	{
		Matrix<T> temp(b);
		int i = 0;
		int j = 0;
		for (; i < m; i++)
			for (; j < n; j++)
			{
				temp->matrix_ptr[i][j] = this->matrix_ptr[i][j] - b->matrix_ptr[i][j];
			}
		temp->show_matrix();
		return temp;
	}
}

template<class T>
Matrix<T> Matrix<T>::operator-()const
{
	int i = 0;
	int j = 0;
	for (; i < m; i++)
		for (; j < n; j++)
		{
			this->matrix_ptr[i][j] = -(this->matrix_ptr[i][j]);
		}
	this->show_matrix();
	return *this;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix& b)const
{
	if (this->judge_zero_vector())
	{
		Matrix<T> a;
		a.show_matrix();
		return a;
	}
	else if (b->judge_zero_vector())
	{
		Matrix<T> a;
		a.show_matrix();
		return a;
	}
	else if (this->n != b->m)
	{
		cout << "�����Ͼ���˷����壡" << endl;
		this->show_matrix();
		return *this;
	}
	else
	{
		Matrix<T> temp(b);
		int i = 0;
		int j = 0;
		for (; i < m; i++)
			for (; j < n; j++)
			{
				temp->matrix_ptr[i][j] = this->matrix_ptr[i][j] * b->matrix_ptr[i][j];
			}
		temp.show_matrix();
		return temp;
	}
}

template<class T>
std::ostream& operator<<(std::ostream& out, const typename Matrix<T>::Matrix& b)
{
	for (int i = 0; i < m; i++)
	{
		out << "(";
		for (int j = 0; j < n - 1; j++)
		{
			out << this->matrix_ptr[i][j] << " ";
		}
		out << this->matrix_ptr[i][n - 1] << ")" << endl;
	}
	return out;
}

template<class T>
std::istream& operator>>(std::istream& in, const typename Matrix<T>::Matrix& b)
{
	for (int i = 0; i < m; i++)
	{
		cout << "(";
		for (int j = 0; j < n - 1; j++)
		{
			in >> this->matrix_ptr[i][j];
			cout << ",";
		}
		in >> this->matrix_ptr[i][n - 1];
		cout << ")" << endl;
	}
	return in;
}

template<class T>
void Matrix<T>::show_matrix()const
{
	for (int i = 0; i < m; i++)
	{
		cout << "(";
		for (int j = 0; j < n - 1; j++)
		{
			cout << this->matrix_ptr[i][j] << " ";
		}
		cout << this->matrix_ptr[i][n - 1] << ")" << endl;
	}
}

template<class T>
bool Matrix<T>::judge_zero_matrix()const
{
	if (this->m == 0 || this->n == 0)
		return true;
	else
		return false;
}

template<class T>
bool Matrix<T>::judge_square_matrix()const//�ж��Ƿ�Ϊ�����ǣ�����true���񣬷���false��
{
	if (this->m == this->n)
		return true;
	else
		return false;
}

template<class T>
T Matrix<T>::determinant(T** matrix_ptr, int n)const
{
	if (n == 1)
	{
		return matrix_ptr[0][0];
	}
	else if (n == 2)//�ݹ��յ㣨��n>=3��
	{
		return matrix_ptr[0][0] * matrix_ptr[1][1] - matrix_ptr[0][1] * matrix_ptr[1][0];
	}
	else
	{
		T matrix_ptr_value = 0;
		for (int k = 0; k < n; k++)
		{
			T** matrix_ptr_temp = new T * [n - 1];//����ʽÿһ�е�һ��Ԫ�أ�����һ��������
			int i2 = 1;//���ݵ�0�е�j��չ����ԭ��������ʽֱ�Ӵӵ�һ�п�ʼ����
			for (int i1 = 0; i1 < n - 1; i1++)//�´���һ����̬��ά���飬���ڴ洢����һ�ξ����ȥһ��һ��Ԫ�غ󣬵���������
			{
				matrix_ptr_temp[i1] = new T[n - 1];//����ʽ��һ��
				int j2 = 0;
				for (int j1 = 0; j1 < n - 1; j1++)
				{
					if (j2 == k)
					{
						j2++;
					}//ȥ����j��
					matrix_ptr_temp[i1][j1] = matrix_ptr[i2][j2++];//��ֵ���µĶ�ά����
				}
				i2++;
			}
			/*	for (int i = 0; i < n-1; i++)
				{
					delete[] matrix_ptr_temp[i];
				}
				delete[] matrix_ptr_temp;*/
			matrix_ptr_value += ((k & 0x0001) ? (-1) : 1) * matrix_ptr[0][k] * determinant(matrix_ptr_temp, n - 1);//�����������ʽ���Ӧ��ĳ˻�,�����к�������
		}
		return matrix_ptr_value;
	}
}

//template<class T>
//Matrix<T> Matrix<T>::inverse_matrix(T** matrix_ptr, T** inverse_matrix_ptr, int n)const//�Լ��ڲ��½�һ��������ά�����    ò�Ʋ�����Ƿ������
//{
//	T determinant = determinant(matrix_ptr, n);
//	if (int(determinant) == 0)
//	{
//		cout << "�þ��󲻿���" << endl;
//		return *this;
//	}
//	else 
//	{
//		if (n == 1)
//		{
//			inverse_matrix_ptr[0][0] = 1 / matrix_ptr[0][0];
//			return;
//		}
//
//		double** adjugate_matrix = new double* [n];
//		for (int i = 0; i < n; i++)
//		{
//			A_adj[i] = new double[n];
//		}
//		adjugate_matrix(matrix_ptr, adjugate_matrix, n);
//		for (int i = 0; i < n; i++)
//		{
//			for (int j = 0; j < n; j++)
//			{
//				inverse_matrix_ptr[i][j] = A_adj[i][j] / A_D;
//			}
//		}
//		for (int i = 0; i < n; i++)
//		{
//			delete[] A_adj[i];
//		}
//		delete[] A_adj;
//	}
//}

//��������
//template<class T>
//void adjugate_matrix(double** matrix_ptr, double** A_adj, int n)
//{
//	T** matrix_for_algebraic_cofactor = new T* [n - 1];//��ʱ��Ŵ�������ʽ�ľ���
//	for (int i = 0; i < n - 1; i++)
//	{
//		matrix_for_algebraic_cofactor[i] = new T[n - 1];//��ʼ������ռ�
//	}
//	for (int x = 0; x < n; x++)
//	{
//		for (int y = 0; y < n; y++)
//		{
//			int i1 = 0;
//			for (int i = 0; i < n - 1; i++)
//			{
//				if (i1 == x)
//				{
//					i1++;
//				}
//				int j1 = 0;
//				for (int j = 0; j < n - 1; j++)
//				{
//					if (j1 == y)
//					{
//						j1++;
//					}
//					matrix_for_algebraic_cofactor[i][j] = matrix_ptr[i1][j1++];
//				}
//				i1++;
//			}//ʵ����ת��
//			A_adj[y][x] = ((x + y) & 0x0001 ? (-1) : 1) * determinant(matrix_for_algebraic_cofactor, n - 1);
//		}
//	}
//	for (int i = 0; i < n - 1; i++)
//	{
//		delete[] matrix_for_algebraic_cofactor[i];
//	}
//	delete[] matrix_for_algebraic_cofactor;
//}

//template<class T>
//Matrix<T> Matrix<T>::transposed_matrix()const//��ת�þ���
//{
//
//}

#endif