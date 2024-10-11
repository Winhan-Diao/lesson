#ifndef VEC_H
#define VEC_H
#include<iostream>
class vec
{
private:
	int dimension;
	double *p;
public:
	vec(const int &length,const double x[]); //一种直接构造
	vec(const int &length=0);	//
	vec(const vec &a);
	vec &operator=(const vec &a);
	vec &operator+(const vec &a)const;
	vec &operator-(const vec &a)const;
	vec &operator+=(const vec &a)const;
	vec &operator-=(const vec &a)const;
	vec &operator*(const vec &a)const;
	vec &operator*=(const vec &a)const;
	bool operator==(const vec &a)const;
	bool operator!=(const vec &a)const;

	double operator[](int &a)const;
	// istream &operator>>(ostream &in,const vec &a);
	void Rand()const;
	void Set()const;
	void show()const;
	~vec();
};


#endif