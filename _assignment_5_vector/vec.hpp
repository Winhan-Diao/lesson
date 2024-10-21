//vec.hpp 由于模板类无法分离编译，采用hpp方法进行模板类的声明和定义
/*
关于此向量的一些额外说明：
1. 向量的维度不能超过100
2. 向量的维度不能为0
3. 0向量专指0维向量，如果是一个n维度向量但值均为0，它仍是一个n维度向量，而不是0向量，
所以仍然不可以参与n维度以外的运算，所以只有当它*0是它成为0向量。这样的设定是因为初始化时
允许一个所有值为0的n维度向量存在，它被保留默认为一个n维向量以供后续使用。
*/
#ifndef VEC_H
#define VEC_H
#include<iostream>
using std::istream;
using std::ostream;
using std::cin;
using std::cout;
using std::endl;
/*
向量夹角
向量模长
0向量
*/
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
	int length()const;
	double modoules()const;
	template<class Ts> friend double angle(const vec<Ts> &a,const vec<Ts> &b);
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
	if(a.dimension!=dimension&&a.dimension!=0&&dimension!=0){
		throw "Add vectors of Different Dimension";
	}
	if(a.dimension==0){
		return *this;
	}
	if(dimension==0) return a;
	vec temp=a;
	for(int i=0;i<dimension;i++){
		temp.p[i]+=a.p[i];
	}
	return temp;
}
vecT vec<T>::operator-(const vec &a) const
{
	if(a.dimension!=dimension&&a.dimension!=0&&dimension!=0){
		throw "Minus vectors of Different Dimension";
	}
	if(a.dimension==0){
		return *this;
	}
	if(dimension==0) {
		vec temp=a;
		for(int i=0;i<dimension;i++){
			temp.p[i]=-a.p[i];
		}
		return temp;
	}
	vec temp=*this;
	for(int i=0;i<dimension;i++){
		temp.p[i]-=a.p[i];
	}
	return temp;
}
vecT &vec<T>::operator+=(const vec &a) 
{
	if(a.dimension!=dimension&&a.dimension!=0&&dimension!=0){
		throw "Add vectors of Different Dimension";
	}
	if(a.dimension==0) return *this;
	if(dimension==0) {
		delete []p;
		dimension=a.dimension;
		p=new T[dimension];
		for(int i=0;i<dimension;i++){
			p[i]=a.p[i];
		}
		return *this;
	}
	for(int i=0;i<dimension;i++)
	{
		p[i]+=a.p[i];
	}
	return *this;
}
vecT &vec<T>::operator-=(const vec &a) 
{
	if(a.dimension!=dimension&&a.dimension!=0&&dimension!=0){
		throw "Minus vectors of Different Dimension";
	}
	if(a.dimension==0) return *this;
	if(dimension==0) {
		delete []p;
		dimension=a.dimension;
		p=new T[dimension];
		for(int i=0;i<dimension;i++){
			p[i]=-a.p[i];
		}
		return *this;
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
	if(a.dimension!=dimension&&a.dimension!=0&&dimension!=0){
		throw "Multiply vectors of Different Dimension";
	}
	if(a.dimension==0) return 0;
	if(dimension==0) return 0;
	for(int i=0;i<dimension;i++){
		temp+=p[i]*a.p[i];
	}
	return temp;
}

template<class Ts,class T>vec<T> operator*(const Ts &a, const vec<T> &v) 
{
	if(a==0){
		return vec<T>(0);
	}
	vec<T> temp=v;
	for(int i=0;i<v.dimension;i++){
		temp.p[i]*=a;
	}
    return temp;
}

template <typename T>
vec<T> vec<T>::operator*(const T &a) const
{
	if(a==0){
		return vec<T>(0);
	}
	vec<T> temp=*this;
	for(int i=0;i<dimension;i++){
		temp.p[i]*=a;
	}
	return temp;
}

template <typename T>
vec<T> &vec<T>::operator*=(const T &a)
{
	if(a==0){
		delete []p;
		dimension=0;
		p=NULL;
		return *this;
	}
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
	if(a<0||a>=dimension) throw "access out of bounds";
	return p[a];
}
template<class T>
istream &operator>>(istream &in,vec<T> &a)
{
	in>>a.dimension;
	if(a.dimension<0||a.dimension>100){
		std::cin.clear(); // 清除错误状态
		std::fflush(stdin);		//防止多输入从而影响其它输入
		throw "Error Dimension number!";
	} 
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
template <class T>
inline double angle(const vec<T> &a, const vec<T> &b)// 计算两个向量的夹角，单位rad
{
	double amod=a.modoules(),bmod=b.modoules();
	if(a.dimension!=b.dimension&&a.dimension!=0&&b.dimension!=0) throw "Error Dimension number!";
	if(a.dimension==0||b.dimension==0||bmod==0||amod==0) return 0;
	double temp=1.0*a*b/(amod*bmod);
	if(temp>1) temp=1;
	else if(temp<-1) temp=-1;
	return acos(temp);
}
template <typename T>
inline int vec<T>::length() const
{
	return dimension;
}
template <typename T>
inline double vec<T>::modoules() const
{
	if(!dimension) return 0;
	double temp=0;
	for(int i=0;i<dimension;i++){
		temp+=p[i]*p[i];
	}
	return sqrt(temp);
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
		return;
	}
	for(int i=0;i<dimension;i++) p[i]=(double)(rand()%10001-5000)/10;
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
			cout<<p[i];
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

