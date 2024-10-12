#include<iostream>
#include<time.h>
#include"vec.h"
//带全部信息的初始化,信息不匹配则抛出f
#define vecT template<typename T>vec<T>
#define typeT template<typename T>
vecT::vec(const int &length,const T x[])
:dimension(length)
{	
	if(length<0||length>100){
		throw "Error Dimension number!";
	}
	if(!length) return;	
	int siz=sizeof(x)/sizeof(T);
	std::cout<<siz;
	if(length>siz){
		throw 'I';
	}
	p=new double[length];

	for(int i=0;i<length;i++){
		p[i]=x[i];
	}
}
//带维度的初始化
vecT::vec(const int &length)
{
	if(length<0||length>100){
		throw "Error Dimension number!";
	}
	dimension=length;
	p=new double[dimension];
	for(int i=0;i<dimension;i++) p[i]=0;
}

vecT::vec(const vec &a)
:dimension(a.dimension)
{
	p=new double[dimension];
	for(int i=0;i<dimension;i++){
		p[i]=a.p[i];
	}
}

vecT &vec<T>::operator=(const vec &a)
{
	if(dimension){
		delete []p;
	}
	dimension=a.dimension;
	p=new double[dimension];
	for(int i=0;i<dimension;i++) p[i]=a.p[i];
	return *this;
}
vecT vec<T>::operator+(const vec&a)const{
	if(a.dimension!=dimension){
		throw "Add vectors of Different Dimension";
	}
	vec temp=a;
	for(int i=0;i<dimension;i++){
		temp.p[i]+=a.p[i];
	}
	return temp;
}
vecT vec<T>::operator-(const vec &a) const
{
	if(a.dimension!=dimension){
		throw "Minus vectors of Different Dimension";
	}
	vec temp=a;
	for(int i=0;i<dimension;i++){
		temp.p[i]-=a.p[i];
	}
	return temp;
}
vecT &vec<T>::operator+=(const vec &a) 
{
	if(a.dimension!=dimension){
		throw "Add vectors of Different Dimension";
	}
	// TODO: 在此处插入 return 语句
}
vecT &vec<T>::operator-=(const vec &a) 
{
	if(a.dimension!=dimension){
		throw "Minus vectors of Different Dimension";
	}
	// TODO: 在此处插入 return 语句
}
typeT
double vec<T>::operator*(const vec &a) const
{

	// TODO: 在此处插入 return 语句
}
typeT
double &vec<T>::operator*=(const vec &a) 
{	

	// TODO: 在此处插入 return 语句
}
typeT
bool vec<T>::operator==(const vec &a) const
{
	return false;
}
typeT
bool vec<T>::operator!=(const vec &a) const
{
	return false;
}
typeT
double vec<T>::operator[](const int &a) const
{
	if(a<=0||a>dimension) throw "access out of bounds";
	return p[a];
}
istream &vec::operator>>(istream &in)
{
	// TODO: 在此处插入 return 语句
}
ostream &vec::operator<<(ostream &out)
{
	// TODO: 在此处插入 return 语句
}
void vec::Set() const
{
	printf("请输入%d个向量值\n",dimension);
	int temp=0;
	while(temp<dimension){
		std::cin>>p[temp++];
	}
}

void vec::Rand()const{//设置为-500~500之间的数
	if(!dimension){
		throw 'R';
	}
	srand(time(0));
	for(int i=0;i<dimension;i++) p[i]=rand()%1001-500;
}
void vec::show()const{
	if(!dimension){
		puts("No dimension!");
		return;
	}else{
		printf("该向量为%d维向量,各个维度分别为：\n",dimension);
		printf("(");
		for(int i=0;i<dimension;i++){
			if(i) printf(" ,");
			printf("%.2lf ",p[i]);
		} 
		printf(")");
		printf("\n");
	}
}
vec::~vec()
{
	if(p!=NULL) delete []p;
	dimension=0;
}