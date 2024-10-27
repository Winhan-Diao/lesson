#include<iostream>
#include<conio.h>
#include<vector>
#include"MathVector.hpp"
#include"color_control.hpp"
#include"Matrix.hpp"
using std::cin;
using std::cout;
using std::endl;
void test();
int Pos(char ch, const char *str);
int Choice(const char *prompt, const char *options);
void IN_test();
void constructors_test();
void calc_test();
int main(){
	// freopen("s.txt","w",stdout);
	Matrix<int> a(3, 3),b;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(i==j) a[i][j]=i+1;
		}
	}
	a.show_matrix();
	cout<<a.determinant(a.a,3)<<endl;
	cout<<a.inverse_matrix()<<endl;
	system("pause");
	test();
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
· 异常抛出测试 √
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
//内置cin>>是比较繁琐的,因为不得不在输入时给予用户提示信息
//如果不提供提示信息又容易导致错误的输入理解，故留给用户自行使用下标引用输入修改
void IN_test(){
	col('b'),cout<<"输入两个向量:"<<endl,col('w');
	MathVector<int> v1,v2;
	col(3),cout<<"输入v1的维度:",col(0);
	int n1,n2;
	cin>>n1;
	v1.resize(n1);
	std::cin.clear(); // 清除错误状态
	std::fflush(stdin);		//防止多输入从而影响其它测试进程。
	col(3),cout<<"输入v1各维度的值:",col(0);
	for(int i=0;i<n1;i++) cin>>v1[i];
	col(3),cout<<"输入v2的维度:",col(0);	
	std::cin.clear(); // 清除错误状态
	std::fflush(stdin);		//防止多输入从而影响其它测试进程。
	cin>>n2;
	v2.resize(n2);	
	std::cin.clear(); // 清除错误状态
	std::fflush(stdin);		//防止多输入从而影响其它测试进程。
	col(3),cout<<"输入v2各维度的值:",col(0);
	for(int i=0;i<n2;i++) cin>>v2[i];
	col('b'),cout<<"v1 v2:",col('w');
	cout<<v1<<"  "<<v2<<endl;
	col('g');
	cout<<"输入重载成功！"<<endl;
	col('w');
	cout<<"测试clear()和clearzero()"<<endl;
	v1.clear();
	v2.clearzero();
	col('b');
	cout<<"v1 v2:",col('w');
	cout<<v1<<"  "<<v2<<endl;
	std::cin.clear(); // 清除错误状态
	std::fflush(stdin);		//防止多输入从而影响其它测试进程。
}
void constructors_test()
{
	// 正常情况测试
	col('b');
	cout<<"正常情况测试："<<endl;
	col('w');
	MathVector<int> v1(5, 10); 
	col('b');
	cout<<"v1(5,10):";
	col('w');
	cout<<v1<<endl;
	MathVector<int> v2(3);
	col('b');
	cout<<"v2(3):";
	col('w');
	cout<<v2<<endl;
    // 边界情况测试
    MathVector<int> v3(0, 0);
	col('b');
	cout<<"v3(0,0)";
	col('w');
	cout<<v3<<endl; 
	MathVector<int> v12=v1;
	col('b');
	cout<<"v12=v1:";
	col('w');
	cout<<v12<<endl;
	//异常情况,考虑到构造函数通常不抛掷异常，弃用
}
//测试 [],=,+,+=,-,-=,*,*=,==,!=,^,^= 运算符的功能
void calc_test(){
	col('b'),cout<<"\n\t1. v1,v2: v1[0]=1,v1[1]=2,v1[2]=3,v2=v1,v4=0 以此对'='和'[]'进行验证"<<endl,col('w');
	MathVector<int> v1(3);
    MathVector<int> v2(3);
	MathVector<int> v3(4,1);
	MathVector<int> v4(0);
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
		v2.at(3);
		flag=1;
	}catch(const std::out_of_range& e){
		cout<<"out_of_range"<<endl;
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
	col('b'),cout<<"v4:",col('w'),cout<<v4<<endl;
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
	col('b'),cout<<"v1+v4:",col('w');
	cout<<v1+v4<<endl;
	col('b'),cout<<"v1+=v4",col('w');
	cout<<v1<<endl;
	col('b'),cout<<"v1-v4",col('w');
	cout<<v1-v4<<endl;
	col('b'),cout<<"v1-=v4",col('w');
	v1-=v4;
	cout<<v1<<endl;
	col('b'),cout<<"v4-=v1",col('w');
	v4-=v1;
	cout<<v4<<endl;
	col('b'),cout<<"v4-=v4",col('w');
	v4-=v4;
	cout<<v4<<endl;
	//异常使用测试
	try{
		col('r');
		cout<<"异常情况测试4,'+'维度不匹配:v1+v3"<<endl;
		col('w');
		v1.plus(v3);
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}
	try{
		col('r');
		cout<<"异常情况测试4,'+='维度不匹配:v1+=v3"<<endl;
		col('w');
		v1.plus_equal(v3);
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}
	try{
		col('r');
		cout<<"异常情况测试4,'-'维度不匹配:v1-v3"<<endl;
		col('w');
		v1.minus(v3);
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}
	try{
		col('r');
		cout<<"异常情况测试4,'-='维度不匹配:v1-=v3"<<endl;
		col('w');
		v1.minus_equal(v3);
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
	col('b'),cout<<"v4:",col('w'),cout<<v4<<endl;
	//正常使用测试
	col('b'),cout<<"v4*0:",col('w');
	cout<<v4*0<<endl;
	col('b'),cout<<"v4=v4*v1",col('w');
	v4=v4*v1;
	cout<<v4<<endl;
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
		v1.dot(v3);
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
		v1.times(v3);
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

	col('b'),cout<<"\n\t6.测试angle()和modoules()运算"<<endl,col('w');
	v1[0]=1,v1[1]=1,v1[2]=1;
	col('b'),cout<<"v1:",col('w'),cout<<v1<<endl;
	col('b'),cout<<"v2:",col('w'),cout<<v2<<endl;
	col('b'),cout<<"v3:",col('w'),cout<<v3<<endl;
	col('b'),cout<<"v4:",col('w'),cout<<v4<<endl;
	col('b'),cout<<"angle(v1,v2):",col('w');
	cout<<angle(v1,v2)<<endl;
	col('b'),cout<<"angle(v1,v4):",col('w');
	cout<<angle(v1,v4)<<endl;
	try{
		col('r');
		cout<<"异常情况测试7,'angle()'维度不适用:v1.angle(v3)"<<endl;
		col('w');
		angle_err(v1,v3);
		flag=1;
	}catch(const char* msg){
		cout<<msg<<endl;
		flag=0;
	}
	if(flag){
		col('r');
		cout<<"'angle()'运算符重载错误"<<endl;
		col('w');
	}else{
		col('g');
		cout<<"'angle()'重载正确"<<endl;
		col('w');
	}
	col('b'),cout<<"v1.modoules():",col('w');
	cout<<v1.modoules()<<endl;
	col('b'),cout<<"v2.modoules():",col('w');
	cout<<v2.modoules()<<endl;
	col('b'),cout<<"v3.modoules():",col('w');
	cout<<v3.modoules()<<endl;
	col('g'),
	cout<<"\n\t~测试完成~"<<endl;
	col('w');
}