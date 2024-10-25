#include"abstract_vector.hpp"
//数学向量类，限定平凡类传入T
template <class T, class Alloc = std::allocator<T>, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
class MathVector:public AbstractVector<T,Alloc>{
protected:
	void expand(size_t requestedExtra = 0) ;//直接继承
public:
	MathVector():AbstractVector<T,Alloc>(){};
	MathVector(size_t n):AbstractVector<T,Alloc>(){
		// expand(n);
		volume=n;
		size=n;
		data=alloc.allocate(n);
		memset(data,0,n*sizeof(T));
	}
	MathVector(const MathVector& c):AbstractVector<T,Alloc>(c){}

	AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>& c)override{
		if(c.getSize()!=this->size){
			throw "size not equal";
		}
		for(size_t i=0;i< this->size;i++){
			this->data[i]+=c[i];
		}
		return *this;
	}      // vector:数值加
    std::unique_ptr<AbstractVector<T, Alloc>> operator+(const AbstractVector<T, Alloc>& c) const override{
		if(c.getSize() !=this->size){
			throw "size not equal";
		}
		MathVector<T,Alloc> res(this->size);
		for(size_t i=0;i<this->size;i++){
			res[i]=this->data[i]+c[i];
		}
		return std::make_unique<MathVector<T, Alloc>>(std::move(res));
	}   

};
