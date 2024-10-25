#include<iostream>
#include"math_vector.hpp"
using std::cin;
using std::cout;
using std::endl;

int main(){
	MathVector<int> v(10);
	
	for(int i=0;i<10;i++){
		v[i]=i;
	}
	system("pause");
	cout<<v<<endl;
	v.pushBack(10);
	cout<<v<<endl;
	v.popBack();
	cout<<v<<endl;
	
	return 0;
}