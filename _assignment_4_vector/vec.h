#ifdef VEC_H
#define VEC_H
#include<iostream>
class vec
{
private:
	int len;
	double *p;
public:
	vec(const int length,const double x[]);
	vec(const int length=0);
	vec(const double x[]);
	void IN();
	~vec();
};


#endif