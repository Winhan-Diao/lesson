/*
"-fexec-charset=GBK",
"-std=c++11",
"-finput-charset=UTF-8"
*/
#include<bits/stdc++.h>
#include <cassert>
#include <conio.h>
#include<windows.h>
using std::cin;
using std::cout;
using std::endl;
#include"vec.hpp"
void col(char msg){
	if(msg=='r') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);//设置红色
	if(msg=='w') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |FOREGROUND_RED |FOREGROUND_GREEN | FOREGROUND_BLUE);//设置三色相加
	if(msg=='g') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);//设置绿色
	if(msg=='b') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);//设置蓝色
}

int Pos(char ch, const char *str);
int Choice(const char *prompt, const char *options="");	// 函数声明,输出提示信息prompt，输入选择的字符并返回。
void test();
void IN_test();
void constructors_test();
void calc_test();
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
		cout<< "a --- 测试向量运算符重载<<,>>,+,+=,-,-=,*,*=,[],==,!=,^,^="<<endl;
		key = Choice("\n请选择", "1234567a\x1b");
		cout << "\n\n";
		if(key==27)		// '\x1b'等于27，指ESC键
			break;
		if(key<'8'&&key>'3'&&!tot){
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
			std::fflush(stdin);//防止多输入
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
			std::fflush(stdin);//防止多输入
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
			test();
			break;
		}  
		default:
			break;
		}
		std::fflush(stdin);	//防止多输入从而影响下一次测试进程。
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
/*
· 直接初始化优化 √
· 不同类型乘法测试 √
· 异常抛出测试 
*/
void test(){
	while(1){
		col('g');
		cout<<"\n\t输入任意键继续测试,输入ESC退出测试!"<<endl;
		col('w');
		cout << "1 --- 输入向量测试" << endl;
		cout << "2 --- 构造函数测试" << endl;
		cout << "3 --- 运算符测试" << endl;
		int key=Choice("\n请选择", "123\x1b");
		if(key==27)break;
		switch (key)
		{
		case '1':{
			col('b');
			cout<<"\n\t\t输入向量测试"<<endl;
			col('w');
			IN_test();
			break;
		}
		case '2':{
			col('b');
			cout<<"\n\t\t构造函数测试"<<endl;
			col('w');	
			constructors_test();
			break;
		}
		case '3':{
			col('b');
			cout<<"\n\t\t运算符测试"<<endl;
			col('w');
			calc_test();
			break;
		}
		default:
			break;
		}
	}
}

void IN_test(){
	try{
		vec<int> v1,v2;
		cin>>v1>>v2;
		cout<<v1<<"  "<<v2<<endl;
	}catch(const char* msg){
		cout<<msg<<endl;
	}
	col('g');
	cout<<"输入重载成功！"<<endl;
	col('w');
	std::fflush(stdin);		//防止多输入从而影响其它测试进程。
}
void constructors_test()
{
	// 正常情况测试
	col('b');
	cout<<"正常情况测试："<<endl;
	col('w');
	vec<int> v1(5, 10); 
	col('b');
	cout<<"v1(5,10):";
	col('w');
	cout<<v1<<endl;
	vec<int> v2(3);
	col('b');
	cout<<"v2(3):";
	col('w');
	cout<<v2<<endl;
    // 边界情况测试
    vec<int> v3(0, 0);
	col('b');
	cout<<"v3(0,0)";
	col('w');
	cout<<v3<<endl; 
	vec<int> v12=v1;
	col('b');
	cout<<"v12=v1:";
	col('w');
	cout<<v12<<endl;
	//异常情况测试
    try {
		col('r');
		cout<<"异常情况测试1,维度负数:v4(-1,0)"<<endl;
		col('w');
        vec<int> v4(-1, 0); 
    } catch (const char* msg) {
		cout<<msg<<endl;
    }
    try {
		col('r');
		cout<<"异常情况测试2,维度过大:v5(101,0)"<<endl;
		col('w');
        vec<int> v5(101, 0); 
    } catch (const char* msg) {
        cout<<msg<<endl;
    }
}

//测试 [],=,+,+=,-,-=,*,*=,==,!=,^,^= 运算符的功能
void calc_test(){
	col('b'),cout<<"\n\t1. v1,v2: v1[0]=1,v1[1]=2,v1[2]=3,v2=v1, 以此对'='和'[]'进行验证"<<endl,col('w');
	vec<int> v1(3);
    vec<int> v2(3);
	vec<int> v3(4,1);
    v1[0] = 1;
    v1[1] = 2;
    v1[2] = 3;
    v2 = v1;
    // 验证赋值后的元素是否正确
    col('b'),cout<<"v1:",col('w');
	cout<<v1<<endl;
	col('b'),cout<<"v2:",col('w');
	cout<<v2<<endl;
	bool flag=0;
    for (int i = 0; i < 3; i++) {
        if(v2[i]!=v1[i]) flag=1;
    }
	try{
		col('r');
		cout<<"异常情况测试3,'[]'下标越界:v2[3]"<<endl;
		col('w');
		v2[3];
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}
    if(flag){ 
		col('r');
		cout<<"赋值运算符重载错误"<<endl;
		col('w');
	}else{
		col('g');
		cout<<"'='和'[]'重载正确"<<endl;
		col('w');
	}

	col('b'),cout<<"\n\t2.测试'+','+=','-','-='运算符"<<endl,col('w');
	col('b'),cout<<"v1:",col('w'),cout<<v1<<endl;
	col('b'),cout<<"v2:",col('w'),cout<<v2<<endl;
	col('b'),cout<<"v3:",col('w'),cout<<v3<<endl;
	//正常使用测试 
	col('b'),cout<<"v1+v2:",col('w');
	cout<<v1+v2<<endl;
	col('b'),cout<<"v1+=v2:",col('w');
	v1+=v2;
	cout<<v1<<endl;
	col('b'),cout<<"v1-v2:",col('w');
	cout<<v1-v2<<endl;
	col('b'),cout<<"v1-=v2:",col('w');
	v1-=v2;
	cout<<v1<<endl;
	
	//异常使用测试
	try{
		col('r');
		cout<<"异常情况测试4,'+'维度不匹配:v1+v3"<<endl;
		col('w');
		v1+v3;
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}
	try{
		col('r');
		cout<<"异常情况测试4,'+='维度不匹配:v1+=v3"<<endl;
		col('w');
		v1+=v3;
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}
	try{
		col('r');
		cout<<"异常情况测试4,'-'维度不匹配:v1-v3"<<endl;
		col('w');
		v1-v3;
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}
	try{
		col('r');
		cout<<"异常情况测试4,'-='维度不匹配:v1-=v3"<<endl;
		col('w');
		v1-=v3;
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}

	if(flag){
		col('r');
		cout<<"'+','+=','-','-='运算符重载错误"<<endl;
		col('w');
	}else{
		col('g');
		cout<<"'+','+=','-','-='重载正确"<<endl;
		col('w');
	}
	
	col('b'),cout<<"\n\t3.测试'*','*='运算符"<<endl,col('w');

	col('b'),cout<<"v1:",col('w'),cout<<v1<<endl;
	col('b'),cout<<"v2:",col('w'),cout<<v2<<endl;
	col('b'),cout<<"v3:",col('w'),cout<<v3<<endl;
	//正常使用测试
	col('b'),cout<<"v1*2,2*v1:",col('w');
	cout<<v1*2<<','<<2*v1<<endl;
	col('b'),cout<<"v1*=2:",col('w');
	v1*=2;
	cout<<v1<<endl;
	col('b'),cout<<"v1*v2:",col('w');
	cout<<v1*v2<<endl;
	//异常使用测试
	try{
		col('r');
		cout<<"异常情况测试5,'*'维度不匹配:v1*v3"<<endl;
		col('w');
		v1*v3;
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}
	if(flag){
		col('r');
		cout<<"'*','*='运算符重载错误"<<endl;
		col('w');
	}else{
		col('g');
		cout<<"'*','*='重载正确"<<endl;
		col('w');
	}

	
	col('b'),cout<<"\n\t4.测试'=='和'!='运算符"<<endl,col('w');

	//正常使用测试
	col('b'),cout<<"v1:",col('w'),cout<<v1<<endl;
	col('b'),cout<<"v2:",col('w'),cout<<v2<<endl;
	col('b'),cout<<"v3:",col('w'),cout<<v3<<endl;
	col('b'),cout<<"v1==v2,v1!=v2,v1==v3:",col('w');
	cout<<(v1==v2)<<','<<(v1!=v2)<<','<<(v1==v3)<<endl;
	col('g'),cout<<"'='和'!='重载正确"<<endl,col('w');

	col('b'),cout<<"\n\t5.测试'^','^='运算符(三维向量的外积)"<<endl,col('w');

	//正常使用测试

	col('b'),cout<<"v1:",col('w'),cout<<v1<<endl;
	col('b'),cout<<"v2:",col('w'),cout<<v2<<endl;
	col('b'),cout<<"v3:",col('w'),cout<<v3<<endl;
	col('b'),cout<<"v1^v2:",col('w');
	cout<<(v1^v2)<<endl;
	col('b'),cout<<"v1^=v2:",col('w');
	v1^=v2;
	cout<<v1<<endl;
	//异常使用测试
	try{
		col('r');
		cout<<"异常情况测试6,'^'维度不适用:v1^v3"<<endl;
		col('w');
		v1^v3;
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}
	if(flag){
		col('r');
		cout<<"'^','^='运算符重载错误"<<endl;
		col('w');
	}else{
		col('g');
		cout<<"'^','^='重载正确"<<endl;
		col('w');
	}
	
	col('g'),
	cout<<"\n\t~测试完成~"<<endl;
	col('w');
}