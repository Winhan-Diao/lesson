#pragma once 
#include"MathVector.hpp"
#include<iostream>
template<class T>
class Mat{
private:
	int row;
	int col;
	MathVector<T>* a;
public:
	Mat(int r,int c):row(r),col(c){
		a=new MathVector<T>[r];
		for(int i=0;i<r;i++){
			a[i].resize(c);
		}
	}
	~Mat(){
		delete[] a;
	}
	MathVector<T>& operator[](int i)const{
		return a[i];
	}
	void show(){
		for(int i=0;i<row;i++){
			for(int j=0;j<col;j++){
				std::cout<<a[i][j]<<" ";
			}
			std::cout<<std::endl;
		}
	}
	void Rand(){
		for(int i=0;i<row;i++) a[i].Rand();
	}
};