#include<iostream>
#include"math_vector.hpp"
using std::cin;
using std::cout;
using std::endl;

int main(){
	// freopen("s.txt","w",stdout);
	MathVector<int> v(10);
	for(int i=0;i<10;i++){
		v[i]=i;
	}	
	MathVector<int> v2=v;
	cout<<v<<endl;
	v.pushBack(10);
	cout<<v<<endl;
	v.popBack();
	cout<<v<<endl;
	cout<<v2<<endl;
	system("pause");
	return 0;
}