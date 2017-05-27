#include<iostream>
#include<fstream>
#include<ctime>
#include"FanseToSAM.h"
using std::cout;
using std::cin;
using std::ifstream;
using std::ofstream;
using std::endl;
int main(int argc,char*argv[]) {
	string log = string(argv[1]);
	log = log.erase(log.length() - 6) + "log";
	ofstream outlog(log);

	if (argc > 2) {
		cout << "输入参数过多，程序退出。";
		outlog << argv[1] << "处理失败，原因：输入参数过多。" << endl;
		exit(EXIT_FAILURE);
	}
	ifstream is(argv[1]);
	if (!is.is_open()) {
		cout << "fanse文件打开失败，请查看输出路径以确认fanse文件在路径中，程序退出。";
		outlog << argv[1] << "处理失败，原因：fanse文件打开失败，请查看输出路径以确认fanse文件在路径中。" << endl;
		exit(EXIT_FAILURE);
	}
	
}