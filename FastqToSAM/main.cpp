#include<iostream>
#include<fstream>
#include<ctime>
#include"FanseToSAM.h"
#include<vector>
using namespace std;
void show_cigar(fanse a,ostream& os);
char* argv[4] = { "aaa","Ex8_ZG_hg38.fanse3","Ex8_ZG.fq","GCA_000001405.22_GRCh38.p7_genomic.fa" };
const int basecount = 80;
int main(){//int argc,char*argv[]) {//1.fanse 2.fastq 3.ref
	string log = string(argv[1]);
	log = log.erase(log.length() - 6) + "log";
	ofstream outlog(log);
	if (!outlog.is_open()) {
		cout << "log�ļ���ʧ�ܣ������˳���";
		exit(EXIT_FAILURE);
	}
	//if (argc > 4) {
		//cout << "����������࣬�����˳���";
		//outlog << argv[1] << "����ʧ�ܣ�ԭ������������ࡣ" << endl;
	//	exit(EXIT_FAILURE);
	//}
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
		exit(EXIT_FAILURE);
	}
	ifstream iref(argv[3]);
	if (!iref.is_open()) {
		cout << "ref�ļ���ʧ�ܣ���鿴����·����ȷ��ref�ļ���·���У������˳���";
		outlog << argv[2] << "����ʧ�ܣ�ԭ��ref�ļ���ʧ�ܣ���鿴����·����ȷ��ref�ļ���·���С�" << endl;
		exit(EXIT_FAILURE);
	}
	string sam = string(argv[1]);
	sam = sam.erase(log.length() - 3) + "sam";
	ofstream outsam(sam);
	if (!outsam.is_open()) {
		cout << "sam�ļ�����ʧ�ܣ������˳���";
		outlog << argv[1] << "sam�ļ�����ʧ�ܣ�����·���Ƿ����/�Ϸ�/����Ȩ���ʡ�" << endl;
		exit(EXIT_FAILURE);
	}
	//ͳ��ÿ��Ⱦɫ���ϵļ����

	vector<chromosome> chr_list;
	char ch;
	string frag;
	chromosome temp2;
	int ch_count = 0;
	int line_count = 0;
	while (iref.get(ch)) {
		if (ch == '>') {
			if (line_count > 0)
				chr_list[ch_count - 1].length = basecount*line_count;
			iref >> temp2.name;
			ch_count++;
			line_count = 0;//renew
			chr_list.push_back(temp2);
		}
		else {
			getline(iref, frag);
			line_count++;
		}
	}
	cout << "Ⱦɫ������ͳ����ɡ�\n";
	//���ע��
	for (int i = 0; i < ch_count; i++) {
		outsam << "@SQ SN:" << chr_list[i].name << " LN:" << chr_list[i].length << endl;
	}
	cout << "ע�������ɡ�\n";
	//�������ѭ����ÿ10^6����Ϣ���һ��
	string waste;
	string qua;
	fanse temp;
	vector<fanse> fanlist;
	int begin = 0;
	int end = 0;

	while(ifan){
		int count = 0;
		while (ifan >> temp.order) {
			ifan >> temp.seq;
			ifan >> temp.mapping;
			ifan >> temp.strand;
			ifan >> waste;
			ifan >> temp.chr;
			temp.chr.erase(temp.chr.find(','));//���δ����м�λ��ѡ�����̣�ֻȡÿ��������mapping���ĵ�һ��λ����Ϊ��mappingλ��
			ifan >> temp.mism;
			ifan >> temp.position;
			ifan >> temp.site_num;
			end = temp.order;
			for (int i = begin; i < end; i++) {//begin��δ����ȡ����ǰһ�е���������1���𣩣�end��Ŀ���е���������1����
				getline(ifsq, waste);
				getline(ifsq, waste);
				getline(ifsq, waste);
				getline(ifsq, qua);
			}
			begin = end;
			temp.quality = qua;
			fanlist.push_back(temp);
			string waste;
			getline(ifan, waste);//ɾ������������Ϣ��ת����һ�С�
			count++;
			if (count > 1000000)
				break;
		}
		cout << "�Ѷ���1000000����Ϣ��\n";
		//���


		for (int i = 0; i < count; i++) {//MAPQ��Ĭ�����100���и��÷���ʱ�滻
			outsam << fanlist[i].order << "	" << 0 << "	" << fanlist[i].chr << "	" << fanlist[i].site_num << "	"
				<< 100 << "	";
			show_cigar(fanlist[i],outsam);//<< "CIGAR here!!";
			outsam << "	" << "*" << "	" << 0 << "	" << 0 << "	" << fanlist[i].seq << "	" << qua;
		}
		cout << "�����1000000����Ϣ��\n";
		fanlist.clear();
		fanlist.swap(fanlist);
	}
	cout << "Done! " << argv[1] << "��ת����ɡ�"<<endl;
	outlog << "Done! " << argv[1] << "��ת����ɡ�" << endl;
	ifan.close();
	iref.close();
	ifsq.close();
	outlog.close();
	return 0;
}

void show_cigar(fanse f,ostream& os)
{
	int count = 1;
	char a=f.mapping[0];
	for (int i = 1; i < f.mapping.length(); i++)
	{
		if (a == f.mapping[i])
			count++;
		else {
			if (a == '.') {//�����ȣ�������
				os << count << '=';
				a = f.mapping[i];
				count = 1;
			}
			else if (a == 'x') {
				os << count << 'X';
				a = f.mapping[i];
				count = 1;
			}
			else if (a == 'A'&&a == 'T'&&a == 'C'&&a == 'G') {
				os << count << 'I';
				a = f.mapping[i];
				count = 1;
			}
			else if (a == '_') {
				os << count << 'D';
				a = f.mapping[i];
				count = 1;
			}
			else if (a == ',')
				break;
		}
	}
	//������ʣ��
	if (a == '.') {
		os << count << '=';
	}
	else if (a == 'x') {
		os << count << 'X';
	}
	else if (a == 'A'&&a == 'T'&&a == 'C'&&a == 'G') {
		os << count << 'I';
	}
	else if (a == '_') {
		os << count << 'D';
	}
	
}
