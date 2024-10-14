//vec.hpp 由于模板类无法分离编译，采用hpp方法进行模板类的声明和定义
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
	vec(const int &length=0);	
	vec(const vec &a);
	vec &operator=(const vec &a);
	vec operator+(const vec &a)const;
	vec operator-(const vec &a)const;
	vec &operator+=(const vec &a);
	vec &operator-=(const vec &a);
	T operator*(const vec &a)const;
	T &operator*=(const vec &a);
	vec operator*(const T &a)const;
	vec &operator*=(const T &a);
	vec operator^(const vec &a)const;
	vec &operator^=(const vec &a);
	bool operator==(const vec &a)const;
	bool operator!=(const vec &a)const;
	T &operator[](const int &a)const;
	istream & operator>>(istream &in);
	ostream & operator<<(ostream &out);
	void length()const;
	void Rand();
	void Set();
	void show()const;
	~vec();
};

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
		throw 'Array length is too short!';
	}
	p=new T[length];

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
	p=new T[dimension];
	for(int i=0;i<dimension;i++) p[i]=0;
}

vecT::vec(const vec &a)
:dimension(a.dimension)
{
	p=new T[dimension];
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
	p=new T[dimension];
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
	for(int i=0;i<dimension;i++)
	{
		p[i]+=a.p[i];
	}
	return *this;
}
vecT &vec<T>::operator-=(const vec &a) 
{
	if(a.dimension!=dimension){
		throw "Minus vectors of Different Dimension";
	}
	for(int i=0;i<dimension;i++)
	{
		p[i]-=a.p[i];
	}
	return *this;
}
typeT
T vec<T>::operator*(const vec &a) const
{
	T temp=0;
	if(a.dimension!=dimension){
		throw "Multiply vectors of Different Dimension";
	}
	for(int i=0;i<dimension;i++){
		temp+=p[i]*a.p[i];
	}
	return temp;
}
typeT
T &vec<T>::operator*=(const vec &a) 
{	
	T temp=0;
	if(a.dimension!=dimension){
		throw "Multiply vectors of Different Dimension";
	}
	for(int i=0;i<dimension;i++){
		temp+=p[i]*a.p[i];
	}
	return temp;
}

template <typename T>
vec<T> vec<T>::operator*(const T &a) const
{
	vec<T> temp=*this;
	for(int i=0;i<dimension;i++){
		temp.p[i]*=a;
	}
	return temp;
}

template <typename T>
vec<T> &vec<T>::operator*=(const T &a)
{
	for(int i=0;i<dimension;i++){
		p[i]*=a;
	}
	return *this;
}

// 向量外积，只能对三维向量进行操作
template <typename T>
vec<T> vec<T>::operator^(const vec &a) const
{
	if(a.dimension!=3||dimension!=3){
		throw std::invalid_argument("Cross product is only defined for 3D vectors.");
	}
	vec<T> temp(3);
	temp[0]=p[1]*a.p[2]-p[2]*a.p[1];
	temp[1]=p[2]*a.p[0]-p[0]*a.p[2];
	temp[2]=p[0]*a.p[1]-p[1]*a.p[0];
	return temp;
}

template <typename T>
vec<T> &vec<T>::operator^=(const vec &a)
{
	if(a.dimension!=3||dimension!=3){
		throw std::invalid_argument("Cross product is only defined for 3D vectors.");
	}
	T tmp=p[0]*a.p[1]-p[1]*a.p[0];
	p[0]=p[1]*a.p[2]-p[2]*a.p[1];
	p[1]=p[2]*a.p[0]-p[0]*a.p[2];
	p[2]=tmp;
	return *this;
}

typeT bool vec<T>::operator==(const vec &a) const
{
	if(a.dimension!=dimension) return false;
	for(int i=0;i<dimension;i++){
		if(p[i]!=a.p[i]) return false;
	}
	return true;
}
typeT
bool vec<T>::operator!=(const vec &a) const
{
	if(a.dimension!=dimension) return true;
	for(int i=0;i<dimension;i++){
		if(p[i]!=a.p[i]) return true;
	}
	return false;
}
typeT
T &vec<T>::operator[](const int &a) const
{
	if(a<=0||a>dimension) throw "access out of bounds";
	return p[a];
}
typeT
istream &vec<T>::operator>>(istream &in)
{
	in>>dimension;
	if(dimension<0||dimension>100) throw "Error Dimension number!";
	p=new T[dimension];
	for(int i=0;i<dimension;i++){
		in>>p[i];
	}
	return in;
}
typeT
ostream &vec<T>::operator<<(ostream &out)
{
	out<<dimension;
	out<<'(';
	for(int i=0;i<dimension;i++){
		out<<p[i];
		if(i!=dimension-1) out<<',';
	}
	out<<')';
	return out;
}
template <typename T>
inline void vec<T>::length() const
{
	std::cout<<dimension<<endl;
}
typeT void vec<T>::Set() 
{
	printf("请输入%d个向量值\n",dimension);
	int temp=0;
	while(temp<dimension){
		std::cin>>p[temp++];
	}
}
typeT
void vec<T>::Rand(){//设置为-500~500之间的数
	if(!dimension){
		throw 'R';
	}
	srand(time(0));
	for(int i=0;i<dimension;i++) p[i]=rand()%1001-500;
}
typeT
void vec<T>::show()const{
	if(!dimension){
		puts("No dimension!");
		return;
	}else{
		printf("该向量为%d维向量,各个维度分别为：\n",dimension);
		printf("(");
		for(int i=0;i<dimension;i++){
			if(i) printf(",");
			printf("%.2lf",p[i]);
		} 
		printf(")");
		printf("\n");
	}
}

template<class T>
vec<T>::~vec()
{
	if(p!=NULL) delete []p;
	dimension=0;
}
#endif