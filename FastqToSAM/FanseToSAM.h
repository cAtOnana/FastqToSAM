#pragma once
#include<iostream>
#include<string>
using std::string;

struct fanse {
	int order;
	string seq;
	string mapping;
	//char strand;
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
