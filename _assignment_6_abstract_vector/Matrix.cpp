#include<iostream>
#include"Matrix.h"
using std::cout;
using std::endl;
using std::cin;

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
	for (int i=0; i < m; i++)
	{
		cout << "(";
		for (int j=0; j < n-1; j++)
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
Matrix<T> Matrix<T>::inverse_matrix()const//�����������
{

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
T Matrix<T>::determinant()const//�����Ӧ����ʽ��ֵ
{

}

template<class T>
Matrix<T> Matrix<T>::transposed_matrix()const//��ת�þ���
{

}





//���½�ѡ��CSDN,�һ�û�п���Щ����

//ʹ�ð��������������
#include <iostream>
//ʹ�ô�������ʽ������ֵ
double determinant_value(double** D, int n)
{
	//�ݹ��յ�
	if (n == 1)
	{
		return  D[0][0];
	}
	else if (n == 2)
	{
		return D[1][1] * D[0][0] - D[0][1] * D[1][0];
	}
	else {
		double D_value = 0;
		for (int k = 0; k < n; k++)
		{
			double** D_temp = new double* [n - 1];
			int i2 = 1;//�����Ǹ��ݵ�0�е�j��չ��������ԭ��������ʽֱ�Ӵӵ�һ�п�ʼ����
			for (int i1 = 0; i1 < n - 1; i1++)
			{
				D_temp[i1] = new double[n - 1];
				int j2 = 0;
				for (int j1 = 0; j1 < n - 1; j1++)
				{
					if (j2 == k)
					{
						j2++;
					}//ȥ����j��
					D_temp[i1][j1] = D[i2][j2++];
				}
				i2++;
			}
			D_value += ((k & 0x0001) ? (-1) : 1) * D[0][k] * determinant_value(D_temp, n - 1);//�����������ʽ���Ӧ��ĳ˻�
		}
		return D_value;
	}
}
//�������� 
void Adjoint_matrices(double** A, double** A_adj, int n)
{
	double** A_AlgebraicRemainder = new double* [n - 1];//��ʱ��Ŵ�������ʽ�ľ���
	for (int i = 0; i < n - 1; i++)
	{
		A_AlgebraicRemainder[i] = new double[n - 1];//��ʼ������ռ�
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
					A_AlgebraicRemainder[i][j] = A[i1][j1++];
				}
				i1++;
			}
			//ʵ����ת��
			A_adj[y][x] = ((x + y) & 0x0001 ? (-1) : 1) * determinant_value(A_AlgebraicRemainder, n - 1);
		}
	}
	for (int i = 0; i < n - 1; i++)
	{
		delete[] A_AlgebraicRemainder[i];
	}
	delete[] A_AlgebraicRemainder;
}

//�������
void matrix_inverse(double** A, double** A_inverse, int n)
{
	double A_D = determinant_value(A, n);

	if (A_D == 0.0f)
	{
		std::cout << "�þ��󲻿���" << std::endl;
		return;
	}
	else {
		if (n == 1)
		{
			A_inverse[0][0] = 1 / A[0][0];
			return;
		}

		double** A_adj = new double* [n];
		for (int i = 0; i < n; i++)
		{
			A_adj[i] = new double[n];
		}
		Adjoint_matrices(A, A_adj, n);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				A_inverse[i][j] = A_adj[i][j] / A_D;
			}
		}
		for (int i = 0; i < n; i++)
		{
			delete[] A_adj[i];
		}
		delete[] A_adj;
	}
}