#ifndef MATRIX_H
#define MATRIX_H
#include<iostream>
//#include"Vector.h"

template<class T>
class Matrix//:public Vector
{
private:
	int m;//矩阵行数（行向量个数）
	int n;//矩阵列数（行向量分量个数）
	T** matrix_ptr=nullptr;
public:
	Matrix();//默认构造函数
	Matrix(const int& m, const int& n);//有参构造函数
	Matrix(const Matrix& copy);//拷贝构造函数
	~Matrix();//析构函数

	Matrix operator+(const Matrix& b)const;//矩阵加法（实现连加）
	Matrix operator-(const Matrix& b)const;//矩阵减法（实现连减）
	Matrix operator-()const;//负矩阵
	Matrix operator*(const Matrix& b)const;//矩阵乘法（实现连乘）
	friend std::ostream& operator<<(std::ostream& out, const Matrix& b);
	friend std::istream& operator>>(std::istream& in, const Matrix& b);

	void show_matrix()const;//展示矩阵
	bool judge_zero_matrix()const;//判断是否为零矩阵（是，返回true；否，返回false）
	Matrix inverse_matrix()const;//求矩阵的逆矩阵
	bool judge_square_matrix()const;//判断是否为方阵（是，返回true；否，返回false）
	T determinant()const;//求方阵对应行列式的值
	Matrix transposed_matrix()const;//求转置矩阵
};

#endif
