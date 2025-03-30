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
	
	string op;//操作符
	string src1;//源操作数1
	string src2;//源操作数2
	string des;//目的操作数
	int id=0;//四元式位置
};
struct Block {
	string name;//基本块的名称
	vector<Quaternary> codes;//基本块中的四元式
	int next1=-1;//基本块的顺序块
	int next2=-1;//基本块的跳转的下一个块
	 // 构造函数，可选地指定基本块名称
	Block(string blkName = "") : name(blkName) {}

	// 添加四元式到基本块
	void addCode(const string& op, const string& arg1, const string& arg2, const string& result,int id=0) {
		codes.push_back({ op, arg1, arg2, result,id });
	}

	// 设置连接
	void setNext(int n1, int n2 = -1) {
		next1 = n1;
		next2 = n2;
	}
};

void outputError(string err);