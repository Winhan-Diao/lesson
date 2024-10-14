/*
"-fexec-charset=GBK",
"-std=c++11",
"-finput-charset=UTF-8"
*/
#include<bits/stdc++.h>
#include <conio.h>
#include<windows.h>
using std::cin;
using std::cout;
using std::endl;
#include"vec.hpp"
int col(char msg){
	if(msg=='r') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);//设置红色
	if(msg=='w') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |FOREGROUND_RED |FOREGROUND_GREEN | FOREGROUND_BLUE);//设置三色相加
	if(msg=='g') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);//设置绿色
	if(msg=='b') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);//设置蓝色
}
int Pos(char ch, const char *str);
int Choice(const char *prompt, const char *options="");	// 函数声明,输出提示信息prompt，输入选择的字符并返回。
void test();
int tot;//向量对象个数
std::vector< vec<double> >q;
int main(){
	int key;
	srand(time(0));
	while(true)
	{
		col('w');
		cout << "\n\n\t向量类模板测试" << endl;
		col('b');cout<<"当前存在"<<tot<<"个向量"<<endl;col('w');
		cout << "1 --- 创建一个随机维度随机值的向量" << endl;
		cout << "2 --- 创建一个指定维度的向量" << endl;
		cout << "3 --- 直接创建一个指定维度的向量并设定各维度值" << endl;
		cout << "4 --- 为已经创建的向量的各个维度值进行设置" << endl;
		cout << "5 --- 为已经存在的向量随机设值" << endl;
		cout<< "6 --- 展示指定编号的向量" <<endl;
		cout<< "7 --- 展示所有存在的向量" <<endl;
		cout<< "a --- 测试向量运算符重载<<,>>,+,+=,-,-=,*,*=,[],==,!="<<endl;
		key = Choice("\n请选择", "1234567a\x1b");
		cout << "\n\n";
		if(key==27)		// '\x1b'等于27，指ESC键
			break;
		if(key>'3'&&!tot){
			col('r');
			cout<<"当前不存在任何向量！";
			col('w');
			continue;
		}
		switch (key)
		{
		case '1':{
			vec<double> x(rand()%10+1);
			x.Rand();
			q.push_back(x);	
			tot++;
			break;			
		}
		case '2':{
			col('g');
			cout<<"输入一个不超过100的正整数n:";
			col('w');
			int n;
			cin>>n;
			if(n<=0||n>100){
				col('r');
				cout<<"维度非法！";
				col('w');
				break;
			}
			vec<double> x(n);
			tot++;
			q.push_back(x);
			break;
		}
		case '3':{
			col('g');
			cout<<"输入一个不超过100的正整数n:";
			col('w');
			int n;
			cin>>n;
			if(n<=0||n>100){
				col('r');
				cout<<"维度非法！";
				col('w');
				break;
			}
			vec<double> x(n);
			tot++;
			q.push_back(x);
			q[tot-1].Set();
			break;
		}
		case '4':{
			int p;
			col('g');
			cout<<"请选择向量编号(编号从1开始)：";
			col('w');
			cin>>p;
			if(p>tot||p<0){
				col('r');
				cout<<"error:不存在的编号！";
				col('w');
				continue;
			}else if(!p){
				break;
			}
			q[p-1].Set();
			break;
		}
		case '5':{
			int p;
			col('g');
			cout<<"请选择向量编号(编号从1开始)：";
			col('w');
			cin>>p;
			if(p>tot||p<0){
				col('r');
				cout<<"error:不存在的编号！";
				col('w');
				continue;
			}else if(!p){
				break;
			}
			q[p-1].Rand();				
			break;
		}
		case '6':{			
			int p;
			col('g');
			cout<<"请选择向量编号(编号从1开始)：";
			col('w');
			cin>>p;
			if(p>tot||p<0){
				col('r');
				cout<<"error:不存在的编号！";
				col('w');
				continue;
			}else if(!p){
				break;
			}
			q[p-1].show();				
			break;
		}
		case '7':{
			int i=0;
			for(auto x: q){
				cout<<"编号"<<++i<<"的向量：\n";
				x.show();
			}
			break;
		}
		case 'a':{
			
			break;
		}  
		default:
			break;
		}
	}
	return 0;
}


int Pos(char ch, const char *str)			// 返回指定字符ch在字符串str中的下标。不存在时返回-1
{
	int i;
	for(i=0; str[i]!='\0'; i++)
		if(ch==str[i])
			return i;
	return -1;
}

int Choice(const char *prompt, const char *options)		// 函数定义。
{
	int key;
	cout << prompt << "{";
	for(int i=0; options[i]!='\0'; i++)
	{
		if(' ' < (options[i] & 0x7f) && (options[i] & 0x7f) < 127)	// 选项options中的可打印字符
			cout << options[i] << ' ';
		else														// 选项options中的部分控制字符
		{
			switch(options[i])
			{
			case '\t':   cout << "[Tab] ";   break;					// 选项中的Tab字符'\t'（即ASCII编码9）
			case '\x1b': cout << "[Esc] ";   break;					// 选项中的Esc字符'\x1b'（即ASCII编码27）
			case ' ':    cout << "[Space] "; break;					// 选项中的空格字符' '（即ASCII编码32）
			}
		}
	}
	cout << "\b}: ";					// '\b'退格。先退一格，在输出右花括号
	do
	{
		key =getch();
	}while(options[0]!='\0' && Pos(key, options)<0); // 若options为空长度为0的字符串，则输入无限制；否则输入项应该在optins中。
	cout << endl;
	return key;
}