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
		cout << "log文件打开失败，程序退出。";
		exit(EXIT_FAILURE);
	}
	//if (argc > 4) {
		//cout << "输入参数过多，程序退出。";
		//outlog << argv[1] << "处理失败，原因：输入参数过多。" << endl;
	//	exit(EXIT_FAILURE);
	//}
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
		exit(EXIT_FAILURE);
	}
	ifstream iref(argv[3]);
	if (!iref.is_open()) {
		cout << "ref文件打开失败，请查看输入路径以确认ref文件在路径中，程序退出。";
		outlog << argv[2] << "处理失败，原因：ref文件打开失败，请查看输入路径以确认ref文件在路径中。" << endl;
		exit(EXIT_FAILURE);
	}
	string sam = string(argv[1]);
	sam = sam.erase(log.length() - 3) + "sam";
	ofstream outsam(sam);
	if (!outsam.is_open()) {
		cout << "sam文件创建失败，程序退出。";
		outlog << argv[1] << "sam文件创建失败，请检查路径是否存在/合法/已授权访问。" << endl;
		exit(EXIT_FAILURE);
	}
	//统计每个染色体上的碱基数

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
	cout << "染色体碱基数统计完成。\n";
	//输出注释
	for (int i = 0; i < ch_count; i++) {
		outsam << "@SQ SN:" << chr_list[i].name << " LN:" << chr_list[i].length << endl;
	}
	cout << "注释输出完成。\n";
	//输入输出循环，每10^6条信息输出一次
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
			temp.chr.erase(temp.chr.find(','));//本次处理中简化位点选择流程，只取每条序列所mapping到的第一个位点作为其mapping位点
			ifan >> temp.mism;
			ifan >> temp.position;
			ifan >> temp.site_num;
			end = temp.order;
			for (int i = begin; i < end; i++) {//begin是未被读取的最前一行的索引（从1算起），end是目标行的索引（从1算起）
				getline(ifsq, waste);
				getline(ifsq, waste);
				getline(ifsq, waste);
				getline(ifsq, qua);
			}
			begin = end;
			temp.quality = qua;
			fanlist.push_back(temp);
			string waste;
			getline(ifan, waste);//删除余下所有信息，转至下一行。
			count++;
			if (count > 1000000)
				break;
		}
		cout << "已读入1000000条信息。\n";
		//输出


		for (int i = 0; i < count; i++) {//MAPQ项默认输出100，有更好方案时替换
			outsam << fanlist[i].order << "	" << 0 << "	" << fanlist[i].chr << "	" << fanlist[i].site_num << "	"
				<< 100 << "	";
			show_cigar(fanlist[i],outsam);//<< "CIGAR here!!";
			outsam << "	" << "*" << "	" << 0 << "	" << 0 << "	" << fanlist[i].seq << "	" << qua;
		}
		cout << "已输出1000000条信息。\n";
		fanlist.clear();
		fanlist.swap(fanlist);
	}
	cout << "Done! " << argv[1] << "已转换完成。"<<endl;
	outlog << "Done! " << argv[1] << "已转换完成。" << endl;
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
			if (a == '.') {//若不等，则清算
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
	//最后输出剩余
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
