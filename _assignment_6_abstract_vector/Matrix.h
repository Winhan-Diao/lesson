#ifndef MATRIX_H
#define MATRIX_H
#include<iostream>
//#include"Vector.h"

template<class T>
class Matrix//:public Vector
{
private:
	int m;//����������������������
	int n;//��������������������������
	T** matrix_ptr=nullptr;
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
	Matrix inverse_matrix()const;//�����������
	bool judge_square_matrix()const;//�ж��Ƿ�Ϊ�����ǣ�����true���񣬷���false��
	T determinant()const;//�����Ӧ����ʽ��ֵ
	Matrix transposed_matrix()const;//��ת�þ���
};

#endif
