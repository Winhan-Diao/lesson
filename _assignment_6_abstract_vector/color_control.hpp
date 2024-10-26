#include <windows.h>
void col(char msg){
	if(msg=='r'||msg==1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);//设置红色
	if(msg=='w'||msg==0) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |FOREGROUND_RED |FOREGROUND_GREEN | FOREGROUND_BLUE);//设置三色相加
	if(msg=='g'||msg==2) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);//设置绿色
	if(msg=='b'||msg==3) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);//设置蓝色
}