#pragma once
#include "common.h"
#include "optimize.h"
//��ǩ��
class NewLabeler {
private:
	int index;
public:
	NewLabeler();
	string newLabel();
};
//�м�����ࣨ��Ԫʽ��
class IntermediateCode {
private:
	vector<Quaternary> code;
	
	NewLabeler nl;
	vector<pair<int, int>> backpatchList; // ��¼��Ҫ�������תָ��λ�ü���Ŀ��
	map<string, int> functionEntries; // ��¼�������λ��
	void output(ostream& out);
	void outputBlocks(ostream& out);
public:
	map<string, vector<Block> >funcBlocks;//������
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
		backpatchList.clear(); // ��ջ����б�
	}
	void merge(const IntermediateCode& other) {
		int offset = code.size(); // ��ǰ��Ԫʽ�����������µ���ʼ���
		code.insert(code.end(), other.code.begin(), other.code.end());
		// ������תλ��
		for (const auto& it : other.backpatchList) {
			addBackpatch(it.first + offset, it.second + offset); // ������תָ���λ�ú�Ŀ���ǩ
		}
		// �ϲ��������λ��
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