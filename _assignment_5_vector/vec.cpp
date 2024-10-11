#include<iostream>
#include<time.h>
#include"vec.h"
//带全部信息的初始化,信息不匹配则抛出f
vec::vec(const int &length,const double x[])
:dimension(length)
{	
	if(length<0){
		throw "Error Dimension number!";
	}
	if(!length) return;	
	int siz=sizeof(x)/sizeof(double);
	std::cout<<siz;
	if(length>siz){
		throw 'I';
	}
	p=new double[length];

	for(int i=0;i<length;i++){
		p[i]=x[i];
	}
}

vec::vec(const int &length)
{
	if(length<0){
		throw "Error Dimension number!";
	}
	dimension=length;
	p=new double[dimension];
	for(int i=0;i<dimension;i++) p[i]=0;
}
vec::vec(const vec &a)
:dimension(a.dimension)
{
	p=new double[dimension];
	for(int i=0;i<dimension;i++){
		p[i]=a.p[i];
	}
}

vec &vec::operator=(const vec &a)
{
	if(dimension){
		delete []p;
	}
	dimension=a.dimension;
	p=new double[dimension];
	for(int i=0;i<dimension;i++) p[i]=a.p[i];
	return *this;
}
vec &vec::operator+(const vec&a){
	
}
void vec::Set()const{
	printf("请输入%d个向量值\n",dimension);
	int temp=0;
	while(temp<dimension){
		std::cin>>p[temp++];
	}
}
void vec::Rand()const{//设置为-500~500之间的数
	if(!dimension){
		throw 'R';
	}
	srand(time(0));
	for(int i=0;i<dimension;i++) p[i]=rand()%1001-500;
}
void vec::show()const{
	if(!dimension){
		puts("No dimension!");
		return;
	}else{
		printf("该向量为%d维向量,各个维度分别为：\n",dimension);
		for(int i=0;i<dimension;i++) printf("%.2lf ",p[i]);
		printf("\n");
	}
}
vec::~vec()
{
	if(p!=NULL) delete []p;
	dimension=0;
}