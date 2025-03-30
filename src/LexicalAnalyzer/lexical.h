#pragma once
#include"common.h"
using namespace std;

struct Symbol {
	string name;//名字
	string type;//类型
	string cat = "v";//种类编码//默认v
	int addr = 0;//地址指针//默认0
	bool declared = false;//声明标识
};
class SymbolTable {
public:
	//用于语义分析时的填充
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
	//词法分析的创建
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
	//得到当前变量的值
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
	//判断符号表是不是空
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
	vector<pair<int, string>> K = { {1,"int"},{2,"void"} ,{3,"if"},{4,"while"},{5,"else"},{6,"return"},{7,"double"},{8,"float"}};//关键字
	SymbolTable I;//标识符
	vector<pair<int, string>> C1;//常整数
	vector<pair<int, string>> C2;//常实数
	vector<pair<int, string>> CT;//字符常量
	vector<pair<int, string>> ST;//字符串常量
	vector<pair<int, string>> P = { {1,"="},{2,"+"} ,{3,"-"}, {4,"*"},{5,"/"}, {6,">"}, {7,"<"},{8,">="},{9,"<="},{10,"!="},{11,","},{12,"("},{13,")"},{14,"{"},{15,"}"},{16,";"},{17,"=="},{18,"//"},{19,"/*"},{20,"*/"} };//界符
	vector<pair<string, int>> Token;//token序列
	void print();//打印token
	ifstream& input;//输入的语句
	int lineCount;//读到的行数
	char cur_char;//当前字符
	string cur_str;//当前解析出来的单词
	int type;//当前字符的类型
public:

	char getNextChar();//得到下一个字符
	void read();//解析单词
	void anal_type();//判断类型
	int isinit(SymbolTable& name);
	int isinit(vector<pair<int, string>>& name);
	bool insert();//根据当前判断进行插入操作(0----标识符，1----关键字，2------界符，3------字符常量，4------字符串常量，5-------常整数，6-----常实数)
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
