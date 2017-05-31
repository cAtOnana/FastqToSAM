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
		cout << "log�ļ���ʧ�ܣ������˳���";
		exit(EXIT_FAILURE);
	}
	if (argc > 3) {
		cout << "����������࣬�����˳���";
		outlog << argv[1] << "����ʧ�ܣ�ԭ������������ࡣ" << endl;
		exit(EXIT_FAILURE);
	}
	ifstream ifan(argv[1]);
	if (!ifan.is_open()) {
		cout << "fanse�ļ���ʧ�ܣ���鿴����·����ȷ��fanse�ļ���·���У������˳���";
		outlog << argv[1] << "����ʧ�ܣ�ԭ��fanse�ļ���ʧ�ܣ���鿴����·����ȷ��fanse�ļ���·���С�" << endl;
		exit(EXIT_FAILURE);
	}
	ifstream ifsq(argv[2]);
	if (!ifsq.is_open()) {
		cout << "fastq�ļ���ʧ�ܣ���鿴����·����ȷ��fastq�ļ���·���У������˳���";
		outlog << argv[2] << "����ʧ�ܣ�ԭ��fastq�ļ���ʧ�ܣ���鿴����·����ȷ��fastq�ļ���·���С�" << endl;
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
		temp.chr.erase(temp.chr.find(','));//���δ����м�λ��ѡ�����̣�ֻȡÿ��������mapping���ĵ�һ��λ����Ϊ��mappingλ��
		ifan >> temp.mism;
		ifan >> temp.position;
		ifan >> temp.site_num;
		temp.quality = qua_list[temp.order - 1];
		fanlist.push_back(temp);
		string waste;
		getline(ifan, waste);//ɾ������������Ϣ��ת����һ�С�
		count++;
	}

}