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
		cout << "����������࣬�����˳���";
		outlog << argv[1] << "����ʧ�ܣ�ԭ������������ࡣ" << endl;
		exit(EXIT_FAILURE);
	}
	ifstream is(argv[1]);
	if (!is.is_open()) {
		cout << "fanse�ļ���ʧ�ܣ���鿴���·����ȷ��fanse�ļ���·���У������˳���";
		outlog << argv[1] << "����ʧ�ܣ�ԭ��fanse�ļ���ʧ�ܣ���鿴���·����ȷ��fanse�ļ���·���С�" << endl;
		exit(EXIT_FAILURE);
	}
	
}