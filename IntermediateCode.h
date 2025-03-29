#pragma once
#include "common.h"
#include "optimize.h"
//标签类
class NewLabeler {
private:
	int index;
public:
	NewLabeler();
	string newLabel();
};
//中间代码类（四元式）
class IntermediateCode {
private:
	vector<Quaternary> code;
	
	NewLabeler nl;
	vector<pair<int, int>> backpatchList; // 记录需要回填的跳转指令位置及其目标
	map<string, int> functionEntries; // 记录函数入口位置
	void output(ostream& out);
	void outputBlocks(ostream& out);
public:
	map<string, vector<Block> >funcBlocks;//基本块
	void emit(Quaternary q);
	void emit(string op, string src1, string src2, string des);
	void back_patch(int index, const string& result);
	void output();
	void output(const char* fileName);
	void divideBlocks();
	void outputBlocks();
	void outputBlocks(const char* fileName);
	int nextQuad();
	void addBackpatch(int index, int target) {
		backpatchList.emplace_back(index, target);
	}
	int backfun(string& name)
	{
		return functionEntries[name];
	}
	void backpatchAll() {
		for (const auto& it : backpatchList) {
			back_patch(it.first, to_string(it.second));
		}
		backpatchList.clear(); // 清空回填列表
	}
	void merge(const IntermediateCode& other) {
		int offset = code.size(); // 当前四元式的数量，即新的起始编号
		code.insert(code.end(), other.code.begin(), other.code.end());
		// 调整跳转位置
		for (const auto& it : other.backpatchList) {
			addBackpatch(it.first + offset, it.second + offset); // 调整跳转指令的位置和目标标签
		}
		// 合并函数入口位置
		for (const auto& it1 : other.functionEntries) {
			functionEntries[it1.first] = it1.second + offset;
		}
	}
	void addFunctionEntry(const string& functionName, int entryPoint) {
		functionEntries[functionName] = entryPoint;
	}
	void setid();
	void optcode()
	{
		for (auto& it : funcBlocks)
		{
			Opt opt(it.second);
			opt.optimizequa();

		}
	}
};