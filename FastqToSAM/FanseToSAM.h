#pragma once
#include<iostream>
#include<string>
using std::string;

struct fanse {
	int order;
	string name;
	string seq;
	string mapping;
	char strand;
	int flag = 0;
	string chr;
	int mism;
	string position;
	int site_num;
	string quality;
};
struct chromosome {
	string name;
	int length=0;
};
