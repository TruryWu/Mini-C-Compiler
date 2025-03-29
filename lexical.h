#pragma once
#include"common.h"
using namespace std;

struct Symbol {
	string name;//����
	string type;//����
	string cat = "v";//�������//Ĭ��v
	int addr = 0;//��ַָ��//Ĭ��0
	bool declared = false;//������ʶ
};
class SymbolTable {
public:
	//�����������ʱ�����
	void assign(string name, string type, string cat, int addr = 0)
	{
		for (int i = 0; i < Symbollist.size(); i++)
		{
			if (Symbollist[i].name == name && Symbollist[i].declared)
			{
				return;
			}
			else if (Symbollist[i].name == name && !Symbollist[i].declared)
			{
				Symbollist[i].declared = true;
				Symbollist[i].type = type;
				Symbollist[i].cat = cat;
				Symbollist[i].addr = addr;
			}
		}
		return;
	}
	//�ʷ������Ĵ���
	int creat(string name, string type = "", string cat = "v", int addr = 0, bool declared = false)
	{
		for (int i = 0; i < Symbollist.size(); i++)
		{
			if (Symbollist[i].name == name)
			{
				return i + 1;
			}
		}
		Symbol a{ name ,type,cat,addr,declared };
		Symbollist.push_back(a);
		return Symbollist.size();
	}
	//�õ���ǰ������ֵ
	int get(string name)
	{
		for (int i = 0; i < Symbollist.size(); i++)
		{
			if (Symbollist[i].name == name)
			{

				return Symbollist[i].addr;
			}
		}
		cout << "Undeclared variable: " + name;
		return -1;
	}
	//�жϷ��ű��ǲ��ǿ�
	bool empty()
	{
		return Symbollist.empty();
	}
	unsigned int size()
	{
		return Symbollist.size();
	}
	Symbol& operator [](int i)
	{
		return Symbollist[i];
	}
	void print()
	{
		for (auto it : Symbollist)
		{
			cout << it.name << "||" << " " << "||" << " " << "||" << " "<<endl;
		}
	}
	vector<Symbol> Symbollist;
};
class Scan {
public:

	Scan(ifstream& a) :input(a)
	{
		lineCount = 0;
		cur_str = "";
		type = 0;
	}
	vector<pair<int, string>> K = { {1,"int"},{2,"void"} ,{3,"if"},{4,"while"},{5,"else"},{6,"return"},{7,"double"},{8,"float"}};//�ؼ���
	SymbolTable I;//��ʶ��
	vector<pair<int, string>> C1;//������
	vector<pair<int, string>> C2;//��ʵ��
	vector<pair<int, string>> CT;//�ַ�����
	vector<pair<int, string>> ST;//�ַ�������
	vector<pair<int, string>> P = { {1,"="},{2,"+"} ,{3,"-"}, {4,"*"},{5,"/"}, {6,">"}, {7,"<"},{8,">="},{9,"<="},{10,"!="},{11,","},{12,"("},{13,")"},{14,"{"},{15,"}"},{16,";"},{17,"=="},{18,"//"},{19,"/*"},{20,"*/"} };//���
	vector<pair<string, int>> Token;//token����
	void print();//��ӡtoken
	ifstream& input;//��������
	int lineCount;//����������
	char cur_char;//��ǰ�ַ�
	string cur_str;//��ǰ���������ĵ���
	int type;//��ǰ�ַ�������
public:

	char getNextChar();//�õ���һ���ַ�
	void read();//��������
	void anal_type();//�ж�����
	int isinit(SymbolTable& name);
	int isinit(vector<pair<int, string>>& name);
	bool insert();//���ݵ�ǰ�жϽ��в������(0----��ʶ����1----�ؼ��֣�2------�����3------�ַ�������4------�ַ���������5-------��������6-----��ʵ��)
};
class Constants {
public:
	Constants() {}
	Constants(string constants);
	void Conter();
	double scientificToDecimal(string s);
	int validateString();

	string x;
	int Int = 0;
	int DoubleLeft = 0;
	double DoubleRight = 0.0;
	bool isHex = false;
	bool isBin = false;
	bool isOct = false;
	int isNum = 3;
	int isconstants = 5;
};
