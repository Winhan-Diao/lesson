#pragma once
#include"abstract_vector.hpp"
/*
特殊声明：
“0向量”专指0维向量，如果是一个n维度向量但值均为0，它仍是一个n维度向量，我们定义
它为"n维空间的0向量"或者"n维度空向量"而不是"0向量"，
n维空向量和一个0向量相比，它仍然不可以参与n维度以外的运算，这样的设定是因为初始化时
允许一个所有值为0的n维度向量存在，它被保留默认为一个n维向量以供后续使用。
数字0不是0向量，是数字标量0
*/
//数学向量类，限定平凡类传入T
template <class T, class Alloc = std::allocator<T>, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
class MathVector:public AbstractVector<T,Alloc>{
public:
	MathVector():AbstractVector<T,Alloc>(){};
	MathVector(size_t n):AbstractVector<T,Alloc>(){
		this->expand(n);
		this->size=n;
		this->data=this->alloc.allocate(n);
		memset(this->data,0,n*sizeof(T));
	}
	MathVector(size_t n,const T &x):AbstractVector<T,Alloc>(){
		this->expand(n);
		this->size=n;
		this->data=this->alloc.allocate(n);
		std::fill(this->begin(),this->end(),x);
	}
	//因为继承后没有新的成员数据需要声明，所以析构和其余构造直接继承
	void resize(const size_t& n=0){//重构,清空并改变大小
		if(n==0){
			clear();
			return;
		}
		this->deleteAll();//释放空间
		this->expand(n);
		this->size=n;
		this->data=this->alloc.allocate(n);
		memset(this->data,0,n*sizeof(T));
	}
	void clearzero(){//清0
		if(this->size==0){
			return;
		}
		memset(this->data,0,this->size*sizeof(T));
	}
	AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>& c)noexcept override{
		if(c.getSize()==0){
			return *this;
		}
		if(this->size==0){//该处继承，子类没办法赋值为基类，只能重新实现构造
			this->expand(c.getSize());
			this->size=c.getSize();
			this->data=this->alloc.allocate(this->size);
			for(size_t i=0;i<this->size;i++) this->data[i]=c[i];
			return *this;
		}
		for(size_t i=0;i< this->size;i++){
			this->data[i]+=c[i];
		}
		return *this;
	}      // +=重载，不带异常检查
	AbstractVector<T,Alloc>& plus_equal(const AbstractVector<T, Alloc>& c){
		if(c.getSize()!=this->size&&c.getSize()!=0&&this->size!=0){
			throw "size not equal";
		}
		return *this+=c;
	}// 加法带异常检查，抛出异常

    MathVector<T,Alloc> operator+(const MathVector<T, Alloc>& c) const noexcept{
		MathVector<T,Alloc> res=*this;
		if(c.getSize()==0){
			return *this;
		}
		if(this->size==0){
			return c;
		}
		for(size_t i=0;i<this->size;i++){
			res[i]+=c[i];
		}
		return res;
	}   //加法无异常，直接返回
	MathVector<T,Alloc> plus(const MathVector<T,Alloc>& c)const{
		if(c.getSize()!=this->size&&c.getSize()!=0&&this->size!=0){
			throw "size not equal";
		}
		return *this+c;
	}//加法带异常检查，抛出异常
	MathVector<T, Alloc>& operator-=(const MathVector<T, Alloc>& c){
		if(c.getSize()==0){
			return *this;
		}
		if(this->size==0){
			*this=c;
			for(size_t i=0;i<this->size;i++) this->data[i]*=-1; 
			return *this;
		}
		for(size_t i=0;i< this->size;i++){
			this->data[i]-=c[i];
		}
		return *this;
	}      // vector:数值减
	MathVector<T,Alloc>& minus_equal(const MathVector<T, Alloc>& c){
		if(c.getSize()!=this->size&&c.getSize()!=0&&this->size!=0){
			throw "size not equal";
		}
		return *this-=c;
	}// 减法带异常检查，抛出异常

	MathVector<T,Alloc> operator-()const noexcept{
		MathVector res=*this;
		for(size_t i=0;i<this->size;i++){
			res[i]*=-1;		
		}
		return res;
	}
	MathVector<T,Alloc> operator-(const MathVector<T, Alloc>& c) const noexcept{
		MathVector<T,Alloc> res(this->size);
		if(c.getSize()==0){
			return *this;
		}
		if(this->size==0){
			res=c;
			for(size_t i=0;i<res.size;i++) res[i]*=-1;
			return res;
		}
		for(size_t i=0;i<this->size;i++){
			res[i]=this->data[i]-c[i];
		}
		return res;
	}   //减法无异常，直接返回
	MathVector<T,Alloc> minus(const MathVector<T,Alloc>& c)const{
		if(c.getSize()!=this->size){
			throw "size not equal";
		}
		return *this-c;
	}//减法带异常检查，抛出异常

	T operator*(const MathVector<T, Alloc>& c)const noexcept{
		T res=0;
		if(c.getSize()==0||this->size==0){
			return 0;
		}
		for(size_t i=0;i<this->size;i++){
			res+=this->data[i]*c[i];
		}
		return res;
	}//点乘无异常检查，直接返回
	T dot(const MathVector<T,Alloc>& c)const{
		if(c.getSize()!=this->size&&c.getSize()!=0&&this->size!=0){
			throw "size not equal";
		}
		return *this*c;
	}//点乘带异常检查，抛出异常
	MathVector<T,Alloc> operator*(const T& c)const noexcept{
		MathVector<T,Alloc> res(this->size);
		for(size_t i=0;i<this->size;i++){
			res[i]=this->data[i]*c;
		}
		return res;
	}//数乘无异常，直接返回
	template<class Ts> friend MathVector<Ts> operator*(const Ts& c,const MathVector<Ts>& v);
	//数字在左边的数乘，返回一个新的向量
	MathVector<T, Alloc>& operator*=(const T& c)noexcept{
		for(size_t i=0;i<this->size;i++){
			this->data[i]*=c;
		}
		return *this;
	}//数乘*=，返回自身
	MathVector<T,Alloc> operator^(const MathVector<T,Alloc>& c)const noexcept{
		MathVector<T,Alloc> res(3);
		res[0]=this->data[1]*c[2]-this->data[2]*c[1];
		res[1]=this->data[2]*c[0]-this->data[0]*c[2];
		res[2]=this->data[0]*c[1]-this->data[1]*c[0];
		return res;
	}// 叉乘无异常，直接返回
	MathVector<T,Alloc> times(const MathVector<T,Alloc>& c)const{
		if(this->size!=3||c.getSize()!=3){
			throw "Cross product is only defined for 3D vectors.";
		}
		return *this^c;
	}// 叉乘带异常，抛出异常
	MathVector<T,Alloc>& operator^=(const MathVector<T,Alloc>& c)noexcept{
		T tmp=this->data[0]*c[1]-this->data[1]*c[0],temp=this->data[0];
		this->data[0]=this->data[1]*c[2]-this->data[2]*c[1];
		this->data[1]=this->data[2]*c[0]-temp*c[2];
		this->data[2]=tmp;
		return *this;
	}// 叉乘^=，不带异常返回自身
	MathVector<T,Alloc>& times_equal(const MathVector<T,Alloc>&c){
		if(this->size!=3||c.getSize()!=3){
			throw "Cross product is only defined for 3D vectors.";
		}
		return *this^=c;
	}// 叉乘^=，带异常返回自身
	bool operator==(const MathVector<T,Alloc>& c)const noexcept{
		if(this->size!=c.getSize()){
			return false;
		}
		for(size_t i=0;i<this->size;i++){
			if(this->data[i]!=c[i]){
				return false;
			}
		}
		return true;
	}
	bool operator!=(const MathVector<T,Alloc>& c)const noexcept{
		return !(*this==c);
	}
	double modoules()const noexcept{
		if(this->size==0){//一个小加速
			return 0;
		}
		double res=0;
		for(size_t i=0;i<this->size;i++){
			res+=this->data[i]*this->data[i];
		}
		return sqrt(res);
	}// 模长无异常，直接返回
	double angle(const MathVector<T,Alloc>& c)const noexcept{
		double mod=this->modoules(),cmod=c.modoules();
		if(mod==0||cmod==0){
			return 0;
		}
		double temp=1.0*(*this*c)/(mod*cmod);
		if(temp>1) temp=1;
		else if(temp<-1) temp=-1;//防止计算过程中由于精度问题产生acos的非法值
		if(temp-1<1e-10) return 0; //temp==1,向量平行
		return acos(temp);
	}// 夹角计算无异常处理，直接返回
	double angle_err(const MathVector<T,Alloc>& c)const{
		if(this->size!=c.getSize()&&this->size!=0&&c.getSize()!=0){
			throw "size not equal";
		}
		return angle(c);
	}// 夹角计算带异常处理，抛出异常
	template<class Ts> friend double angle(const MathVector<Ts> &a,const MathVector<Ts> &b)noexcept;
	template<class Ts> friend double angle_err(const MathVector<Ts> &a,const MathVector<Ts> &b);
	void Rand(){
		if(this->size==0) return;
		for(size_t i=0;i<this->size;i++){
			this->data[i]=1.0*(rand()*rand()%100001-50000)/100;
		}//-500.00~500.00的随机数
	}
};
template <class Ts>
inline MathVector<Ts> operator*(const Ts &c, const MathVector<Ts> &v)
{
	return v*c;
}
template <class Ts>
double angle(const MathVector<Ts> &a, const MathVector<Ts> &b)noexcept{
	return a.angle(b);
}
template<class Ts>
double angle_err(const MathVector<Ts> &a, const MathVector<Ts> &b){
	return a.angle_err(b);
}