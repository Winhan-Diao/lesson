#pragma once
#include"abstract_vector.hpp"
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

	AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>& c)override{
		if(c.getSize()!=this->size){
			throw "size not equal";
		}
		for(size_t i=0;i< this->size;i++){
			this->data[i]+=c[i];
		}
		return *this;
	}      // vector:数值加
    MathVector<T,Alloc> operator+(const AbstractVector<T, Alloc>& c) const{
		if(c.getSize() !=this->size){
			throw "size not equal";
		}
		MathVector<T,Alloc> res(this->size);
		for(size_t i=0;i<this->size;i++){
			res[i]=this->data[i]+c[i];
		}
		return res;
	}   
	
};
