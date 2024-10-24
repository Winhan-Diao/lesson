#include"abstract_vector.hpp"

template <class T, class Alloc = std::allocator<T>>
class MathVector:public AbstractVector<T,Alloc>{
protected:
	void expand(size_t requestedExtra = 0) ;//直接继承
public:
	MathVector():AbstractVector<T,Alloc>(data(nullptr),size(0),volume(0)){};
	MathVector(size_t n):AbstractVector<T,Alloc>(data(nullptr),size(n),volume(n)){}; 

};