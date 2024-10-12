#ifndef VEC_H
#define VEC_H
#include<iostream>
using std::istream;
using std::ostream;
template<typename T>
class vec
{
private:
	int dimension;
	T *p;
public:
	vec(const int &length,const T x[]); //一种直接构造
	vec(const int &length=0);	//
	vec(const vec &a);
	vec &operator=(const vec &a);
	vec operator+(const vec &a)const;
	vec operator-(const vec &a)const;
	vec &operator+=(const vec &a);
	vec &operator-=(const vec &a);
	T operator*(const vec &a)const;
	T &operator*=(const vec &a);
	bool operator==(const vec &a)const;
	bool operator!=(const vec &a)const;
	T operator[](const int &a)const;
	istream & operator>>(istream &in);
	ostream & operator<<(ostream &out);
	void Rand()const;
	void Set()const;
	void show()const;
	~vec();
};


#endif