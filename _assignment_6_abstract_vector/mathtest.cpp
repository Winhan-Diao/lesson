#include<iostream>
#include"math_vector.hpp"
using std::cin;
using std::cout;
using std::endl;

int main(){
	// freopen("s.txt","w",stdout);
	MathVector<int> v(3);
	for(int i=0;i<3;i++){
		v[i]=i;
	}	
	MathVector<int> v2(3);
	cout<<v<<endl;
	v.pushBack(10);
	cout<<v<<endl;
	v.popBack();
	cout<<v.modoules()<<endl;
	cout<<v<<endl;
	cout<<v2<<endl;
	cout<<v+v2<<endl;
	cout<<v-v2<<endl;
	cout<<v2-v<<endl;
	cout<<v2+v<<endl;
	v2+=v;
	v2-=v;
	cout<<v*v2<<endl;
	cout<<(v^v2)<<endl;
	cout<<v2*2<<endl;
	cout<<(2*v2)<<endl;
	cout<<v2*0<<endl;
	cout<<0*v2<<endl;
	cout<<(v==v2)<<endl;
	cout<<(v!=v2)<<endl;
	cout<<v.angle(v2)<<endl;
	v.Rand();
	cout<<v;
	system("pause");
	return 0;
}