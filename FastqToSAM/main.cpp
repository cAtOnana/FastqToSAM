#include<iostream>
#include<fstream>
#include<ctime>
#include"FanseToSAM.h"
#include<vector>
using namespace std;
int main(int argc,char*argv[]) {
	string log = string(argv[1]);
	log = log.erase(log.length() - 6) + "log";
	ofstream outlog(log);
	if (!outlog.is_open()) {
		cout << "log文件打开失败，程序退出。";
		exit(EXIT_FAILURE);
	}
	if (argc > 3) {
		cout << "输入参数过多，程序退出。";
		outlog << argv[1] << "处理失败，原因：输入参数过多。" << endl;
		exit(EXIT_FAILURE);
	}
	ifstream ifan(argv[1]);
	if (!ifan.is_open()) {
		cout << "fanse文件打开失败，请查看输入路径以确认fanse文件在路径中，程序退出。";
		outlog << argv[1] << "处理失败，原因：fanse文件打开失败，请查看输入路径以确认fanse文件在路径中。" << endl;
		exit(EXIT_FAILURE);
	}
	ifstream ifsq(argv[2]);
	if (!ifsq.is_open()) {
		cout << "fastq文件打开失败，请查看输入路径以确认fastq文件在路径中，程序退出。";
		outlog << argv[2] << "处理失败，原因：fastq文件打开失败，请查看输入路径以确认fastq文件在路径中。" << endl;
	}
	vector<string> qua_list;
	string waste;
	string qua;
	while (getline(ifsq,waste)) {
		getline(ifsq,waste);
		getline(ifsq, waste);
		getline(ifsq, qua);
		qua_list.push_back(qua);
	}
	vector<fanse> fanlist;
	int count = 0;
	fanse temp;
	while (ifan >> temp.order) {
		ifan >> temp.seq;
		ifan >> temp.mapping;
		ifan >> temp.strand;
		ifan >> temp.chr;
		temp.chr.erase(temp.chr.find(','));//本次处理中简化位点选择流程，只取每条序列所mapping到的第一个位点作为其mapping位点
		ifan >> temp.mism;
		ifan >> temp.position;
		ifan >> temp.site_num;
		temp.quality = qua_list[temp.order - 1];
		fanlist.push_back(temp);
		string waste;
		getline(ifan, waste);//删除余下所有信息，转至下一行。
		count++;
	}

}