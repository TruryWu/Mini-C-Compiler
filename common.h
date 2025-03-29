#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <cstdlib>
#include <cassert>
#include <functional>
using namespace std;

struct Quaternary {
	
	string op;//������
	string src1;//Դ������1
	string src2;//Դ������2
	string des;//Ŀ�Ĳ�����
	int id=0;//��Ԫʽλ��
};
struct Block {
	string name;//�����������
	vector<Quaternary> codes;//�������е���Ԫʽ
	int next1=-1;//�������˳���
	int next2=-1;//���������ת����һ����
	 // ���캯������ѡ��ָ������������
	Block(string blkName = "") : name(blkName) {}

	// �����Ԫʽ��������
	void addCode(const string& op, const string& arg1, const string& arg2, const string& result,int id=0) {
		codes.push_back({ op, arg1, arg2, result,id });
	}

	// ��������
	void setNext(int n1, int n2 = -1) {
		next1 = n1;
		next2 = n2;
	}
};

void outputError(string err);