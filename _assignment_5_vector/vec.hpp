//vec.hpp 由于模板类无法分离编译，采用hpp方法进行模板类的声明和定义
#ifndef VEC_H
#define VEC_H
#include<iostream>
using std::istream;
using std::ostream;
using std::cin;
using std::cout;
template<typename T>
class vec
{
private:
	int dimension;
	T *p;
public:
	vec(const int &length=0,const T &x=0); //带默认值构造函数
	vec(const vec &a);
	vec &operator=(const vec &a);
	vec operator+(const vec &a)const;
	vec operator-(const vec &a)const;
	vec &operator+=(const vec &a);
	vec &operator-=(const vec &a);
	T operator*(const vec &a)const;
	vec operator*(const T &a)const;
	vec &operator*=(const T &a);
	template<class Ts,class Tv> friend vec<Tv> operator*(const Ts &a,const vec<Tv> &v);
	vec operator^(const vec &a)const;
	vec &operator^=(const vec &a);
	bool operator==(const vec &a)const;
	bool operator!=(const vec &a)const;
	T &operator[](const int &a)const;
	template<typename Ts> friend istream &operator>>(istream &in,vec<Ts> &a);
	template<typename Ts> friend ostream &operator<<(ostream &out,const vec<Ts> &a);
	void length()const;
	void Rand();
	void Set();
	void show()const;
	~vec();
};

#define vecT template<typename T>vec<T>
#define typeT template<typename T>
template <typename T>
inline vec<T>::vec(const int &length, const T &x)
:dimension(length)
{
	if(length<0||length>100){
		throw "Error Dimension number!";
	}
	p=new T[dimension];
	for(int i=0;i<dimension;i++) p[i]=x;
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
	vec temp=*this;
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

template<class Ts,class T>vec<T> operator*(const Ts &a, const vec<T> &v) 
{
	vec<T> temp=v;
	for(int i=0;i<v.dimension;i++){
		temp.p[i]*=a;
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
		throw "Cross product is only defined for 3D vectors.";
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
		throw "Cross product is only defined for 3D vectors.";
	}
	T tmp=p[0]*a.p[1]-p[1]*a.p[0],temp=p[0];
	p[0]=p[1]*a.p[2]-p[2]*a.p[1];
	p[1]=p[2]*a.p[0]-temp*a.p[2];
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
	if(a<0||a>dimension) throw "access out of bounds";
	return p[a];
}
template<class T>
istream &operator>>(istream &in,vec<T> &a)
{
	in>>a.dimension;
	if(a.dimension<0||a.dimension>100) throw "Error Dimension number!";
	a.p=new T[a.dimension];
	for(int i=0;i<a.dimension;i++){
		in>>a.p[i];
	}
	return in;
}
template <typename T>
ostream &operator<<(ostream &out,const vec<T> &a)
{
	out<<'(';
	for(int i=0;i<a.dimension;i++){
		out<<a.p[i];
		if(i!=a.dimension-1) out<<',';
	}
	out<<")_{"<<a.dimension<<'}';
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
		throw "empty vector!";
	}
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

