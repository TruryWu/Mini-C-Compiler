#pragma once
#include"lexical.h"
#include"selectAndtable.h"
#include "IntermediateCode.h"
using namespace std;
class SemanticValue {
public:
	bool isVt;//是否为终结符
	string content;//内容
	IntermediateCode quadruples; // 存储四元式
	friend bool operator ==(const SemanticValue& one, const SemanticValue& other);
	friend bool operator < (const SemanticValue& one, const SemanticValue& other);
	SemanticValue(const SemanticValue& sym);
	SemanticValue(const bool& isVt, const string& content);
	SemanticValue();
};


//数据类型（int/void）
enum DType { D_VOID, D_INT ,D_DOUBLE , D_FLOAT };

struct Var {
	string name;
	DType type;
	int level;
};

struct Func {
	string name;//函数名
	DType returnType;//返回值
	list<DType> paramTypes;//参数表的值
	int enterPoint;//入口地址
};

class Id :public SemanticValue {
public:
	string name;//变量名
	Id(const SemanticValue& sym, const string& name);
};

class Num :public SemanticValue {
public:
	string number;//常数的值
	Num(const SemanticValue& sym, const string& number);
};

class Type :public SemanticValue {
public:
	string type;//常数的值
	Type (const SemanticValue& sym, const string& type);
};

class FunctionDeclare :public SemanticValue {
public:
	list<DType>plist;//参数类型列表
	FunctionDeclare(const SemanticValue& sym);
};

class Parameter :public SemanticValue {
public:
	list<DType>plist;//参数类型列表
	Parameter(const SemanticValue& sym);
};

class ParameterList :public SemanticValue {
public:
	list<DType>plist;//参数类型列表
	ParameterList(const SemanticValue& sym);
};

class SentenceBlock :public SemanticValue {
public:
	//list<int>nextList;//跳转目标列表
	SentenceBlock(const SemanticValue& sym);
};

class SentenceList :public SemanticValue {
public:
	//list<int>nextList;//跳转目标列表
	SentenceList(const SemanticValue& sym);
};

class Sentence :public SemanticValue {
public:
	//list<int>nextList;//跳转目标列表
	Sentence(const SemanticValue& sym);
};

class WhileSentence :public SemanticValue {
public:
	//list<int>nextList;//跳转目标列表
	WhileSentence(const SemanticValue& sym);
};

class IfSentence :public SemanticValue {
public:
	//list<int>nextList;//跳转目标列表
	IfSentence(const SemanticValue& sym);
};

class Expression :public SemanticValue {
public:
	string name;//表达式的值
	//list<int>falseList;//假跳转列表
	Expression(const SemanticValue& sym);
};

class M :public SemanticValue {
public:
	int quad;//四元式的位置
	M(const SemanticValue& sym);
};

class N :public SemanticValue {
public:
	N(const SemanticValue& sym);
};

class AddExpression :public SemanticValue {
public:
	string name;//表达式的值
	AddExpression(const SemanticValue& sym);
};

class Nomial :public SemanticValue {
public:
	string name;//单项式的值
	Nomial(const SemanticValue& sym);
};

class Factor :public SemanticValue {
public:
	string name;//因子的值
	Factor(const SemanticValue& sym);
};

class ArgumentList :public SemanticValue {
public:
	list<string> alist;//参数值列表
	ArgumentList(const SemanticValue& sym);
};

class NewTemper {
private:
	int now;
public:
	NewTemper();
	string newTemp();
};
class  Parser {
private:
	int lineCount;
	int nowLevel;//当前分析行所在的语句块级次
	Scan& scanner; // 词法分析器
	GrammarAnalyzerLR analy;//得到控制表
	vector<Rule> grammar;   // 文法规则
	map<int, map<string, pair<string, int>>> actionTable;//操作表
	map<int, map<string, int>> gotoTable;//移动表
	vector<pair<string, int>> Token;//token序列
	SymbolTable symboltable;//词法分析得到的符号表
	stack<SemanticValue*> symStack;//符号栈
	stack<int> staStack;//状态栈
	size_t cur_index;//当前Token序列的位置
	vector<Var> varTable;//变量表
	vector<Func> funcTable;//函数表
	IntermediateCode code;//四元式
	SymbolTable intervartable;//中间变量表
	NewTemper nt;//建立新变量
	Func* lookUpFunc(string ID);//寻找函数
	Var* lookUpVar(string ID);//寻找变量
	SemanticValue* popSymbol();
	list<int> my_merge(list<int>& l1, list<int>& l2) {
		list<int>ret;
		ret.assign(l1.begin(), l1.end());
		ret.splice(ret.end(), l2);
		return ret;
	}
	void pushSymbol(SemanticValue* sym);
	bool march(list<string>& argument_list, list<DType>& parameter_list);
	//查表
	pair<string, int> cheakTable(int state, string str, map<int, map<string, pair<string, int>>> table)
	{
		auto it1 = table.find(state);
		if (it1 != table.end())
		{
			auto it2 = it1->second.find(str);
			if (it2 != it1->second.end())
			{
				return it2->second;
			}
		}
		return make_pair("err", 0);
	}
	int cheakTable(int state, string str, map<int, map<string, int>> table)
	{
		auto it1 = table.find(state);
		if (it1 != table.end())
		{
			auto it2 = it1->second.find(str);
			if (it2 != it1->second.end())
			{
				return it2->second;
			}
		}
		return -1;
	}
	//查找Token序列原本的值
	string findvalue(string tokenType, int position)
	{
		if (tokenType == "K") {
			return scanner.K[position - 1].second;
		}
		else if (tokenType == "I") {
			return scanner.I.Symbollist[position - 1].name;
		}
		else if (tokenType == "C1") {
			return scanner.C1[position - 1].second;
		}
		else if (tokenType == "C2") {
			return scanner.C2[position - 1].second;
		}
		else if (tokenType == "CT") {
			return scanner.CT[position - 1].second;

		}
		else if (tokenType == "ST") {
			return scanner.ST[position - 1].second;
		}
		else if (tokenType == "P") {
			return scanner.P[position - 1].second;
		}
		else if (tokenType == "$")
		{
			return "$";
		}
		return "";
	}
public:
	//构造函数
	Parser(Scan& scanner) : scanner(scanner), cur_index(0) {
		Token = scanner.Token;
		symboltable = scanner.I;
		lineCount = 0;
		analy.readFromFile("ActionAndGoto.txt");
		actionTable = analy.actionTable;
		gotoTable = analy.gotoTable;
		grammar = analy.getgrammar();
		Token.push_back(make_pair("$", 0));
	}
	//判断
	void Judge() {
		bool acc = false;
		symStack.push(new SemanticValue(true, "$"));
		staStack.push(0);
		while (cur_index < Token.size()) {

			string LT = Token[cur_index].first;

			//忽略行注释和段注释
			if (LT == "LCOMMENT" || LT == "PCOMMENT") {
				cur_index++;
				continue;
			}
			if (LT == "NEXTLINE") {
				lineCount++;
				cur_index++;
				continue;
			}
			string word = findvalue(LT, Token[cur_index].second);
			SemanticValue* nextSemanticValue;
			if (LT == "I") {
				nextSemanticValue = new Id(SemanticValue{ true,"ID" }, word);
			}
			else if (LT == "C1" || LT == "C2") {
				nextSemanticValue = new Num(SemanticValue{ true,"NUM" }, word);
			}
			else if(word=="int"||word=="double"||word=="float") {
				nextSemanticValue = new Type(SemanticValue{ true,"TYPE" }, word);
			}
			else
			{
				nextSemanticValue = new SemanticValue(SemanticValue(true, word));
			}
			if (cheakTable(staStack.top(), nextSemanticValue->content, actionTable).first == "err") {

				outputError(string("语法错误：第") + to_string(lineCount) + "行，不期待的符号" + nextSemanticValue->content);
			}

			auto bh = cheakTable(staStack.top(), nextSemanticValue->content, actionTable);
			if (bh.first == "s") {
				symStack.push(nextSemanticValue);
				staStack.push(bh.second);
				cur_index++;
			}
			else if (bh.first == "r") {
				Rule reductPro = grammar[bh.second];

				int popSymNum = reductPro.rhs.size();
				switch (bh.second) {
				case 1: // <P> -> <N> <declare_list>
				{
					SemanticValue* declare_list = popSymbol();
					N* n = (N*)popSymbol();
					SemanticValue* P = new SemanticValue(false, reductPro.lhs);
					P->quadruples.merge(n->quadruples);
					P->quadruples.merge(declare_list->quadruples);
					code = P->quadruples;
					pushSymbol(P);
					break;
				}

				case 2: // <declare_list> -> <declare> <declare_list>
				{
					SemanticValue* declare_list = popSymbol();
					SemanticValue* declare = popSymbol();
					SemanticValue* declare_list1 = new SemanticValue(false, reductPro.lhs);
					declare_list1->quadruples.merge(declare->quadruples);
					declare_list1->quadruples.merge(declare_list->quadruples);
					pushSymbol(declare_list1);
					break;
				}

				case 3: // <declare_list> -> <declare>
				{
					SemanticValue* declare = popSymbol();
					SemanticValue* declare_list = new SemanticValue(false, reductPro.lhs);
					declare_list->quadruples.merge(declare->quadruples);
					pushSymbol(declare_list);
					break;
				}
				case 4://declare ::= TYPE ID M A function_declare
				{
					FunctionDeclare* function_declare = (FunctionDeclare*)popSymbol();
					SemanticValue* A = popSymbol();
					M* m = (M*)popSymbol();
					Id* ID = (Id*)popSymbol();
					Type* type = (Type*)popSymbol();
					if (lookUpFunc(ID->name) == NULL)
					{
						if (type->type == "int")
						{
							Func newfun{ ID->name, D_INT, function_declare->plist };
							funcTable.push_back(newfun);
						}else if (type->type == "double")
						{
							Func newfun{ ID->name, D_DOUBLE, function_declare->plist };
							funcTable.push_back(newfun);
						}
						else
						{
							Func newfun{ ID->name, D_FLOAT, function_declare->plist };
							funcTable.push_back(newfun);
						}
					}
					else
					{
						outputError(string("语法错误：第") + to_string(lineCount) + "行，函数重复声明" + ID->name);
					}
					SemanticValue* declare = new SemanticValue(false, reductPro.lhs);
					int interfun = declare->quadruples.nextQuad();
					declare->quadruples.emit("function", ID->name, "_", "_");
					declare->quadruples.addFunctionEntry(ID->name, interfun);
					declare->quadruples.merge(ID->quadruples);
					declare->quadruples.merge(m->quadruples);
					declare->quadruples.merge(A->quadruples);
					declare->quadruples.merge(function_declare->quadruples);
					declare->quadruples.emit("function_end", ID->name, "_", "_");
					pushSymbol(declare);
					break;
				}
				case 5://declare ::= TYPE ID var_declare
				{
					SemanticValue* var_declare = popSymbol();
					Id* ID = (Id*)popSymbol();
					Type* type = (Type*)popSymbol();
					if (lookUpVar(ID->name)==NULL)
					{
						if (type->type == "int")
						{
							varTable.push_back(Var{ ID->name, D_INT, nowLevel });
						}
						else if (type->type == "double")
						{
							varTable.push_back(Var{ ID->name, D_DOUBLE, nowLevel });
						}
						else
						{
							varTable.push_back(Var{ ID->name, D_FLOAT, nowLevel });
						}
					}
					else
					{
						outputError(string("语法错误：第") + to_string(lineCount) + "行，变量重复声明" + ID->name);
					}
					SemanticValue* declare = new SemanticValue(false, reductPro.lhs);
					declare->quadruples.emit(type->type, ID->name, "_", "_");
					declare->quadruples.merge(var_declare->quadruples);
					pushSymbol(declare);
					break;
				}
				case 6://declare ::= void ID M A function_declare
				{
					FunctionDeclare* function_declare = (FunctionDeclare*)popSymbol();
					SemanticValue* A = popSymbol();
					M* m = (M*)popSymbol();
					Id* ID = (Id*)popSymbol();
					SemanticValue* _void = popSymbol();
					if (lookUpFunc(ID->name) == NULL)
					{
                       funcTable.push_back(Func{ ID->name, D_VOID, function_declare->plist });
					}
					else
					{
						outputError(string("语法错误：第") + to_string(lineCount) + "行，函数重复声明" + ID->name);
					}
					
					SemanticValue* declare = new SemanticValue(false, reductPro.lhs);
					int interfun = declare->quadruples.nextQuad();
					declare->quadruples.emit("function", ID->name, "_", "_");
					declare->quadruples.addFunctionEntry(ID->name, interfun);
					declare->quadruples.merge(ID->quadruples);
					declare->quadruples.merge(m->quadruples);
					declare->quadruples.merge(A->quadruples);
					declare->quadruples.merge(function_declare->quadruples);
					declare->quadruples.emit("function_end", ID->name, "_", "_");
					pushSymbol(declare);
					break;
				}
				case 7://A ::=
				{
					nowLevel++;
					SemanticValue* A = new SemanticValue(false, reductPro.lhs);
					pushSymbol(A);
					break;
				}
				case 9://function_declare ::= ( parameter ) sentence_block
				{
					SentenceBlock* sentence_block = (SentenceBlock*)popSymbol();
					SemanticValue* rparen = popSymbol();
					Parameter* parameter = (Parameter*)popSymbol();
					SemanticValue* lparen = popSymbol();
					FunctionDeclare* function_declare = new FunctionDeclare(SemanticValue(false, reductPro.lhs));
					function_declare->plist.assign(parameter->plist.begin(), parameter->plist.end());
					function_declare->quadruples.merge(parameter->quadruples);
					function_declare->quadruples.merge(sentence_block->quadruples);
					pushSymbol(function_declare);
					break;
				}
				case 10://parameter :: = parameter_list
				{
					ParameterList* parameter_list = (ParameterList*)popSymbol();
					Parameter* parameter = new Parameter(SemanticValue(false, reductPro.lhs));
					parameter->plist.assign(parameter_list->plist.begin(), parameter_list->plist.end());
					parameter->quadruples.merge(parameter_list->quadruples);
					pushSymbol(parameter);
					break;
				}
				case 11://parameter ::= void
				{
					SemanticValue* _void = popSymbol();
					Parameter* parameter = new Parameter(SemanticValue(false, reductPro.lhs));
					pushSymbol(parameter);
					break;
				}
				case 12://parameter_list ::= param
				{
					SemanticValue* param = popSymbol();
					ParameterList* parameter_list = new ParameterList(SemanticValue(false, reductPro.lhs));
					parameter_list->plist.push_back(D_INT);
					parameter_list->quadruples.merge(param->quadruples);
					pushSymbol(parameter_list);
					break;
				}
				case 13://parameter_list1 ::= param , parameter_list2
				{
					ParameterList* parameter_list2 = (ParameterList*)popSymbol();
					SemanticValue* comma = popSymbol();
					SemanticValue* param = popSymbol();
					ParameterList* parameter_list1 = new ParameterList(SemanticValue(false, reductPro.lhs));
					parameter_list2->plist.push_front(D_INT);
					parameter_list1->plist.assign(parameter_list2->plist.begin(), parameter_list2->plist.end());
					parameter_list1->quadruples.merge(parameter_list2->quadruples);
					parameter_list1->quadruples.merge(param->quadruples);
					pushSymbol(parameter_list1);
					break;
				}
				case 14://param ::= TYPE ID
				{
					Id* ID = (Id*)popSymbol();
					Type* type = (Type*)popSymbol();
					if(type->type =="int")
					{
						varTable.push_back(Var{ ID->name, D_INT, nowLevel });
					}
					else if (type->type == "double")
					{
						varTable.push_back(Var{ ID->name, D_DOUBLE, nowLevel });
					}
					else
					{
						varTable.push_back(Var{ ID->name, D_FLOAT, nowLevel });
					}
					
					SemanticValue* param = new SemanticValue(false, reductPro.lhs);
					param->quadruples.emit("get", "_", "_", ID->name);
					pushSymbol(param);
					break;
				}
				case 15://sentence_block ::= { inner_declare sentence_list }
				{
					SemanticValue* rbrace = popSymbol();
					SentenceList* sentence_list = (SentenceList*)popSymbol();

					SemanticValue* inner_declare = popSymbol();
					SemanticValue* lbrace = popSymbol();
					SentenceBlock* sentence_block = new SentenceBlock(SemanticValue(false, reductPro.lhs));
					sentence_block->quadruples.merge(inner_declare->quadruples);
					sentence_block->quadruples.merge(sentence_list->quadruples);
					nowLevel++;
					int popNum = 0;
					for (auto riter = varTable.rbegin(); riter != varTable.rend(); ++riter) {
						if (riter->level > nowLevel)
							popNum++;
						else
							break;
					}
					for (int i = 0; i < popNum; i++) {
						varTable.pop_back();
					}
					pushSymbol(sentence_block);
					break;
				}
				case 17:
				{//<inner_declare> -> <inner_var_declare>; <inner_declare>1
					SemanticValue* inner_declare1 = popSymbol();
					SemanticValue* quid = popSymbol();
					SemanticValue* inner_var_declare = popSymbol();
					SemanticValue* inner_declare = new SemanticValue(false, reductPro.lhs);
					inner_declare->quadruples.merge(inner_var_declare->quadruples);
					inner_declare->quadruples.merge(inner_declare1->quadruples);
					pushSymbol(inner_declare);
					break;
				}
				case 18://inner_var_declare ::= TYPE ID
				{
					Id* ID = (Id*)popSymbol();
					Type* type = (Type*)popSymbol();
					SemanticValue* inner_var_declare = new SemanticValue(false, reductPro.lhs);
					if (lookUpVar(ID->name) == NULL) {
						if (type->type == "int")
						{
							Var newvar = Var{ ID->name, D_INT, nowLevel };
							varTable.push_back(newvar);
						}
						else if (type->type == "double")
						{
							Var newvar = Var{ ID->name, D_DOUBLE, nowLevel };
							varTable.push_back(newvar);
						}
						else
						{
							Var newvar = Var{ ID->name, D_FLOAT, nowLevel };
							varTable.push_back(newvar);
						}
					}
					else
					{
						outputError(string("语法错误：第") + to_string(lineCount) + "行，变量" + ID->name + "重复声明");
					}
					inner_var_declare->quadruples.emit(type->type, ID->name, "_", "_");
					pushSymbol(inner_var_declare);
					break;
				}
				case 19://sentence_list ::= sentence M sentence_list
				{
					SentenceList* sentence_list2 = (SentenceList*)popSymbol();
					M* m = (M*)popSymbol();
					Sentence* sentence = (Sentence*)popSymbol();
					SentenceList* sentence_list1 = new SentenceList(SemanticValue(false, reductPro.lhs));
					sentence_list1->quadruples.merge(sentence->quadruples);
					sentence_list1->quadruples.merge(m->quadruples);
					sentence_list1->quadruples.merge(sentence_list2->quadruples);
					pushSymbol(sentence_list1);
					break;
				}
				case 20://sentence_list ::= sentence
				{
					Sentence* sentence = (Sentence*)popSymbol();
					SentenceList* sentence_list = new SentenceList(SemanticValue(false, reductPro.lhs));
					sentence_list->quadruples.merge(sentence->quadruples);
					pushSymbol(sentence_list);
					break;
				}
				case 21://sentence ::= if_sentence
				{
					IfSentence* if_sentence = (IfSentence*)popSymbol();
					Sentence* sentence = new Sentence(SemanticValue(false, reductPro.lhs));
					sentence->quadruples.merge(if_sentence->quadruples);
					pushSymbol(sentence);
					break;
				}
				case 22://sentence ::= while_sentence
				{
					WhileSentence* while_sentence = (WhileSentence*)popSymbol();
					Sentence* sentence = new Sentence(SemanticValue(false, reductPro.lhs));
					sentence->quadruples.merge(while_sentence->quadruples);
					pushSymbol(sentence);
					break;
				}
				case 23://sentence ::= return_sentence
				{
					SemanticValue* return_sentence = popSymbol();
					Sentence* sentence = new Sentence(SemanticValue(false, reductPro.lhs));
					sentence->quadruples.merge(return_sentence->quadruples);
					pushSymbol(sentence);
					break;
				}
				case 24://sentence ::= assign_sentence
				{
					SemanticValue* assign_sentence = popSymbol();
					Sentence* sentence = new Sentence(SemanticValue(false, reductPro.lhs));
					sentence->quadruples.merge(assign_sentence->quadruples);
					pushSymbol(sentence);
					break;
				}
				case 25://assign_sentence ::= ID = expression ;
				{
					SemanticValue* comma = popSymbol();
					Expression* expression = (Expression*)popSymbol();
					SemanticValue* assign = popSymbol();
					Id* ID = (Id*)popSymbol();
					if (lookUpVar(ID->name) == NULL) {
						outputError(string("语法错误：第") + to_string(lineCount) + "行，变量" + ID->name + "未声明");
					}
					SemanticValue* assign_sentence = new SemanticValue(SemanticValue(false, reductPro.lhs));
					assign_sentence->quadruples.merge(expression->quadruples);
					assign_sentence->quadruples.emit("=", expression->name, "_", ID->name);
					pushSymbol(assign_sentence);
					break;
				}
				case 26://return_sentence ::= return ;
				{
					SemanticValue* comma = popSymbol();
					SemanticValue* _return = popSymbol();
					SemanticValue* return_sentence = new SemanticValue(SemanticValue(false, reductPro.lhs));
					return_sentence->quadruples.emit("return", "_", "_", "_");
					pushSymbol(return_sentence);
					break;
				}
				case 27://return_sentence ::= return expression ;
				{
					SemanticValue* comma = popSymbol();
					Expression* expression = (Expression*)popSymbol();
					SemanticValue* _return = popSymbol();
					SemanticValue* return_sentence = new SemanticValue(SemanticValue(false, reductPro.lhs));
					return_sentence->quadruples.merge(expression->quadruples);
					return_sentence->quadruples.emit("return", expression->name, "_", "_");
					pushSymbol(return_sentence);
					break;
				}
				case 28://while_sentence ::= while M ( expression ) A sentence_block
				{
					SentenceBlock* sentence_block = (SentenceBlock*)popSymbol();
					SemanticValue* A = popSymbol();
					SemanticValue* rparen = popSymbol();
					Expression* expression = (Expression*)popSymbol();
					SemanticValue* lparen = popSymbol();
					M* m = (M*)popSymbol();
					SemanticValue* _while = popSymbol();
					WhileSentence* while_sentence = new WhileSentence(SemanticValue(false, reductPro.lhs));
					while_sentence->quadruples.emit("wh", "_", "_", "_");
					while_sentence->quadruples.merge(expression->quadruples);
					int jmpFalseIndex = while_sentence->quadruples.nextQuad();
					while_sentence->quadruples.emit("do", expression->name, "_", "_");
					while_sentence->quadruples.merge(sentence_block->quadruples);
					while_sentence->quadruples.addBackpatch(jmpFalseIndex, while_sentence->quadruples.nextQuad());
					while_sentence->quadruples.emit("we", "_", "_", "_");
					pushSymbol(while_sentence);

					break;
				}
				case 29://if_sentence ::= if ( expression ) A sentence_block
				{
					SentenceBlock* sentence_block = (SentenceBlock*)popSymbol();
					SemanticValue* A = popSymbol();
					SemanticValue* rparen = popSymbol();
					Expression* expression = (Expression*)popSymbol();
					SemanticValue* lparen = popSymbol();
					SemanticValue* _if = popSymbol();
					IfSentence* if_sentence = new IfSentence(SemanticValue(false, reductPro.lhs));
					if_sentence->quadruples.merge(expression->quadruples);
					int jmpFalseIndex = if_sentence->quadruples.nextQuad();
					if_sentence->quadruples.emit("if", "_", "_", expression->name);
					if_sentence->quadruples.merge(sentence_block->quadruples);
					if_sentence->quadruples.addBackpatch(jmpFalseIndex, if_sentence->quadruples.nextQuad()); // 加入需要回填的列表
					if_sentence->quadruples.emit("ie", "_", "_", "_");
					pushSymbol(if_sentence);
					break;
				}
				case 30://if_sentence ::= if ( expression ) A1 sentence_block1 N else M A2 sentence_block2
				{
					SentenceBlock* sentence_block2 = (SentenceBlock*)popSymbol();
					SemanticValue* A2 = popSymbol();
					M* m = (M*)popSymbol();
					SemanticValue* _else = popSymbol();
					N* n = (N*)popSymbol();
					SentenceBlock* sentence_block1 = (SentenceBlock*)popSymbol();
					SemanticValue* A1 = popSymbol();
					SemanticValue* rparen = popSymbol();
					Expression* expression = (Expression*)popSymbol();
					SemanticValue* lparen = popSymbol();
					SemanticValue* _if = popSymbol();
					IfSentence* if_sentence = new IfSentence(SemanticValue(false, reductPro.lhs));
					//将expression中的四元式加入if中
					if_sentence->quadruples.merge(expression->quadruples);
					//得到失败的跳转四元式位置
					int jmpFalseIndex = if_sentence->quadruples.nextQuad();
					if_sentence->quadruples.emit("if_start", expression->name, "_", "_");
					if_sentence->quadruples.merge(sentence_block1->quadruples);
					//得到结束的跳转位置
					int jmpEndIndex = if_sentence->quadruples.nextQuad();
					if_sentence->quadruples.addBackpatch(jmpFalseIndex, if_sentence->quadruples.nextQuad()); // 加入需要回填的列表
					if_sentence->quadruples.emit("else", "_", "_", "_");
					if_sentence->quadruples.merge(n->quadruples);
					if_sentence->quadruples.merge(sentence_block2->quadruples);
					if_sentence->quadruples.addBackpatch(jmpEndIndex, if_sentence->quadruples.nextQuad()); // 加入需要回填的列表
					if_sentence->quadruples.emit("if_end", "_", "_", "_");
					pushSymbol(if_sentence);
					break;
				}
				case 31://N ::= 
				{
					N* n = new N(SemanticValue(false, reductPro.lhs));
					pushSymbol(n);
					break;
				}
				case 32://M ::=
				{
					M* m = new M(SemanticValue(false, reductPro.lhs));
					pushSymbol(m);
					break;
				}
				case 33://expression ::= add_expression
				{
					AddExpression* add_expression = (AddExpression*)popSymbol();
					Expression* expression = new Expression(SemanticValue(false, reductPro.lhs));
					expression->name = add_expression->name;
					expression->quadruples.merge(add_expression->quadruples);
					pushSymbol(expression);
					break;
				}
				case 34://expression ::= add_expression1 > add_expression2
				{
					AddExpression* add_expression2 = (AddExpression*)popSymbol();
					SemanticValue* gt = popSymbol();
					AddExpression* add_expression1 = (AddExpression*)popSymbol();
					Expression* expression = new Expression(SemanticValue(false, reductPro.lhs));
					expression->name = nt.newTemp();
					intervartable.creat(expression->name, "v", "int", 1,true);
					expression->quadruples.merge(add_expression1->quadruples);
					expression->quadruples.merge(add_expression2->quadruples);
					expression->quadruples.emit(">", add_expression1->name, add_expression2->name, expression->name);
					pushSymbol(expression);
					break;
				}
				case 35://expression ::= add_expression1 < add_expression2
				{
					AddExpression* add_expression2 = (AddExpression*)popSymbol();
					SemanticValue* lt = popSymbol();
					AddExpression* add_expression1 = (AddExpression*)popSymbol();
					Expression* expression = new Expression(SemanticValue(false, reductPro.lhs));
					expression->name = nt.newTemp();
					intervartable.creat(expression->name, "v", "int", 1,true);
					expression->quadruples.merge(add_expression1->quadruples);
					expression->quadruples.merge(add_expression2->quadruples);
					expression->quadruples.emit("<", add_expression1->name, add_expression2->name, expression->name);
					pushSymbol(expression);
					break;
				}
				case 36://expression ::= add_expression1 == add_expression2
				{
					AddExpression* add_expression2 = (AddExpression*)popSymbol();
					SemanticValue* eq = popSymbol();
					AddExpression* add_expression1 = (AddExpression*)popSymbol();
					Expression* expression = new Expression(SemanticValue(false, reductPro.lhs));
					expression->name = nt.newTemp();
					intervartable.creat(expression->name, "v", "int", 1,true);
					expression->quadruples.merge(add_expression1->quadruples);
					expression->quadruples.merge(add_expression2->quadruples);
					expression->quadruples.emit("==", add_expression1->name, add_expression2->name, expression->name);
					pushSymbol(expression);
					break;
				}
				case 37://expression ::= add_expression1 >= add_expression2
				{
					AddExpression* add_expression2 = (AddExpression*)popSymbol();
					SemanticValue* get = popSymbol();
					AddExpression* add_expression1 = (AddExpression*)popSymbol();
					Expression* expression = new Expression(SemanticValue(false, reductPro.lhs));
					expression->name = nt.newTemp();
					intervartable.creat(expression->name, "v", "int", 1,true);
					expression->quadruples.merge(add_expression1->quadruples);
					expression->quadruples.merge(add_expression2->quadruples);
					expression->quadruples.emit(">=", add_expression1->name, add_expression2->name, "-1");
					pushSymbol(expression);
					break;
				}
				case 38://expression ::= add_expression1 <= add_expression2
				{
					AddExpression* add_expression2 = (AddExpression*)popSymbol();
					SemanticValue* let = popSymbol();
					AddExpression* add_expression1 = (AddExpression*)popSymbol();
					Expression* expression = new Expression(SemanticValue(false, reductPro.lhs));
					expression->name = nt.newTemp();
					intervartable.creat(expression->name, "v", "int", 1,true);
					expression->quadruples.merge(add_expression1->quadruples);
					expression->quadruples.merge(add_expression2->quadruples);
					expression->quadruples.emit("<=", add_expression1->name, add_expression2->name, expression->name);
					pushSymbol(expression);
					break;
				}
				case 39://expression ::= add_expression1 != add_expression2
				{
					AddExpression* add_expression2 = (AddExpression*)popSymbol();
					SemanticValue* neq = popSymbol();
					AddExpression* add_expression1 = (AddExpression*)popSymbol();
					Expression* expression = new Expression(SemanticValue(false, reductPro.lhs));
					expression->name = nt.newTemp();
					intervartable.creat(expression->name, "v", "int", 1,true);
					expression->quadruples.merge(add_expression1->quadruples);
					expression->quadruples.merge(add_expression2->quadruples);
					expression->quadruples.emit("!=", add_expression1->name, add_expression2->name, expression->name);
					pushSymbol(expression);
					break;
				}
				case 40://add_expression ::= item
				{
					Nomial* item = (Nomial*)popSymbol();
					AddExpression* add_expression = new AddExpression(SemanticValue(false, reductPro.lhs));
					add_expression->name = item->name;
					add_expression->quadruples.merge(item->quadruples);
					pushSymbol(add_expression);
					break;
				}
				case 41://add_expression1 ::= item + add_expression2
				{
					AddExpression* add_expression2 = (AddExpression*)popSymbol();
					SemanticValue* add = popSymbol();
					Nomial* item = (Nomial*)popSymbol();
					AddExpression* add_expression1 = new AddExpression(SemanticValue(false, reductPro.lhs));
					add_expression1->name = nt.newTemp();
					
					Var* addt= lookUpVar(add_expression2->name)==NULL? lookUpVar(item->name): lookUpVar(add_expression2->name);
					string type;
					switch (addt->type)
					{
					case 0:
					{
						type = "void";
						break;
					}
					case 1:
					{
						type = "int";
						break;
					}case 2:
					{
						type = "double";
						break;
					}case 3:
					{
						type = "float";
						break;
					}
					}
					intervartable.creat(add_expression1->name, "v", type, 1,true);
					add_expression1->quadruples.merge(item->quadruples);
					add_expression1->quadruples.merge(add_expression2->quadruples);
					add_expression1->quadruples.emit("+", item->name, add_expression2->name, add_expression1->name);
					pushSymbol(add_expression1);
					break;
				}
				case 42://add_expression ::= item - add_expression
				{
					AddExpression* add_expression2 = (AddExpression*)popSymbol();
					SemanticValue* sub = popSymbol();
					Nomial* item = (Nomial*)popSymbol();
					AddExpression* add_expression1 = new AddExpression(SemanticValue(false, reductPro.lhs));
					add_expression1->name = nt.newTemp();
					Var* addt = lookUpVar(add_expression2->name) == NULL ? lookUpVar(item->name) : lookUpVar(add_expression2->name);;
					string type;
					switch (addt->type)
					{
					case 0:
					{
						type = "void";
						break;
					}
					case 1:
					{
						type = "int";
						break;
					}case 2:
					{
						type = "double";
						break;
					}case 3:
					{
						type = "float";
						break;
					}
					}
					intervartable.creat(add_expression1->name, "v", type, 1,true);
					add_expression1->quadruples.merge(item->quadruples);
					add_expression1->quadruples.merge(add_expression2->quadruples);
					add_expression1->quadruples.emit("-", item->name, add_expression2->name, add_expression1->name);
					pushSymbol(add_expression1);

					break;
				}
				case 43://item ::= factor
				{
					Factor* factor = (Factor*)popSymbol();
					Nomial* item = new Nomial(SemanticValue(false, reductPro.lhs));
					item->name = factor->name;
					item->quadruples.merge(factor->quadruples);
					pushSymbol(item);
					break;
				}
				case 44://item1 ::= factor * item2
				{
					Nomial* item2 = (Nomial*)popSymbol();
					SemanticValue* mul = popSymbol();
					Factor* factor = (Factor*)popSymbol();
					Nomial* item1 = new Nomial(SemanticValue(false, reductPro.lhs));
					item1->name = nt.newTemp();
					Var* addt = lookUpVar(factor->name) == NULL ? lookUpVar(factor->name) : lookUpVar(item2->name);;
					string type;
					switch (addt->type)
					{
					case 0:
					{
						type = "void";
						break;
					}
					case 1:
					{
						type = "int";
						break;
					}case 2:
					{
						type = "double";
						break;
					}case 3:
					{
						type = "float";
						break;
					}
					}
					intervartable.creat(item1->name, "v", type, 1,true);
					item1->quadruples.merge(factor->quadruples);
					item1->quadruples.merge(item2->quadruples);
					item1->quadruples.emit("*", factor->name, item2->name, item1->name);
					pushSymbol(item1);
					break;
				}
				case 45://item1 ::= factor / item2
				{
					Nomial* item2 = (Nomial*)popSymbol();
					SemanticValue* div = popSymbol();
					Factor* factor = (Factor*)popSymbol();
					Nomial* item1 = new Nomial(SemanticValue(false, reductPro.lhs));
					item1->name = nt.newTemp();
					string type;
					Var* addt = lookUpVar(factor->name) == NULL ? lookUpVar(factor->name) : lookUpVar(item2->name);;
					switch (addt->type)
					{
					case 0:
					{
						type = "void";
						break;
					}
					case 1:
					{
						type = "int";
						break;
					}case 2:
					{
						type = "double";
						break;
					}case 3:
					{
						type = "float";
						break;
					}
					}
					intervartable.creat(item1->name, "v", type, 1,true);
					item1->quadruples.merge(factor->quadruples);
					item1->quadruples.merge(item2->quadruples);
					item1->quadruples.emit("/", factor->name, item2->name, item1->name);
					pushSymbol(item1);
					break;
				}
				case 46://factor ::= NUM
				{
					Num* num = (Num*)popSymbol();
					Factor* factor = new Factor(SemanticValue(false, reductPro.lhs));
					factor->name = num->number;
					pushSymbol(factor);
					break;
				}
				case 47://factor ::= ( expression )
				{
					SemanticValue* rparen = popSymbol();
					Expression* expression = (Expression*)popSymbol();
					SemanticValue* lparen = popSymbol();
					Factor* factor = new Factor(SemanticValue(false, reductPro.lhs));
					factor->name = expression->name;
					factor->quadruples.merge(expression->quadruples);
					pushSymbol(factor);
					break;
				}
				case 48://factor ::= ID ( argument_list )
				{
					SemanticValue* rparen = popSymbol();
					ArgumentList* argument_list = (ArgumentList*)popSymbol();
					SemanticValue* lparen = popSymbol();
					Id* ID = (Id*)popSymbol();
					Factor* factor = new Factor(SemanticValue(false, reductPro.lhs));
					Func* f = lookUpFunc(ID->name);
					if (!f) {
						outputError(string("语法错误：第") + to_string(lineCount) + "行，未声明的函数" + ID->name);
					}
					else if (!march(argument_list->alist, f->paramTypes)) {
						outputError(string("语法错误：第") + to_string(lineCount) + "行，函数" + ID->name + "参数不匹配");
					}
					else {
						for (auto& arg : argument_list->alist) {
							factor->quadruples.emit("par", arg, "_", "_");
						}
						factor->name = nt.newTemp();
						string type;
						Func* addt = lookUpFunc(ID->name);
						switch (addt->returnType)
						{
						case 0:
						{
							type = "void";
							break;
						}
						case 1:
						{
							type = "int";
							break;
						}case 2:
						{
							type = "double";
							break;
						}case 3:
						{
							type = "float";
							break;
						}
						}
						intervartable.creat(factor->name, "v", type, 1,true);
						factor->quadruples.emit("call", ID->name, "_", "_");
						factor->quadruples.emit("=", "@RETURN_PLACE", "_", factor->name);
					}
					factor->quadruples.merge(argument_list->quadruples);
					pushSymbol(factor);
					break;
				}
				case 49://factor ::= ID
				{
					Id* ID = (Id*)popSymbol();
					if (lookUpVar(ID->name) == NULL) {
						outputError(string("语法错误：第") + to_string(lineCount) + "行，变量" + ID->name + "未声明");
					}
					Factor* factor = new Factor(SemanticValue(false, reductPro.lhs));
					factor->name = ID->name;

					pushSymbol(factor);
					break;
				}
				case 50://argument_list ::= 
				{
					ArgumentList* argument_list = new ArgumentList(SemanticValue(false, reductPro.lhs));
					pushSymbol(argument_list);
					break;
				}
				case 51://argument_list ::= expression
				{
					Expression* expression = (Expression*)popSymbol();
					ArgumentList* argument_list = new ArgumentList(SemanticValue(false, reductPro.lhs));
					argument_list->alist.push_back(expression->name);
					argument_list->quadruples.merge(expression->quadruples);
					pushSymbol(argument_list);
					break;
				}
				case 52://argument_list1 ::= expression , argument_list2
				{
					ArgumentList* argument_list2 = (ArgumentList*)popSymbol();
					SemanticValue* comma = popSymbol();
					Expression* expression = (Expression*)popSymbol();
					ArgumentList* argument_list1 = new ArgumentList(SemanticValue(false, reductPro.lhs));
					argument_list2->alist.push_front(expression->name);
					argument_list1->alist.assign(argument_list2->alist.begin(), argument_list2->alist.end());
					argument_list1->quadruples.merge(argument_list2->quadruples);
					argument_list1->quadruples.merge(expression->quadruples);
					pushSymbol(argument_list1);
					break;
				}
				default:
					if (reductPro.rhs[0] != "@")
					{
						for (int i = 0; i < popSymNum; i++) {
							popSymbol();
						}
					}
					pushSymbol(new SemanticValue(false, reductPro.lhs));
					break;
				}
			}
			else if (bh.first == "acc") {//P ::= N declare_list
				acc = true;
				//回填函数入口
				backpathfun();
				//回填跳转地址
				code.backpatchAll();
				//将语义符号表和词法符号表合并
				mergesymtable();
				code.setid();
				break;
			}
		}
		if (!acc) {
			outputError("语法错误：未知的结尾");
		}
	}
	//打印符号表
	void printval(SymbolTable symboltable)
	{
		for (auto it : symboltable.Symbollist)
		{
			cout << it.name << " " << it.type << " " << it.cat<<" "<<it.addr << endl;
		}
	}
    //回填函数入口
	void backpathfun()
	{
		for (auto &it : funcTable)
		{
			it.enterPoint = code.backfun(it.name);
		}
	}
	//回填符号表
	void mergesymtable()
	{
		for (auto it : varTable)
		{
			string type;
			switch (it.type)
			{
			case 0:
			{
				type = "void";
				break;
			}
			case 1:
			{
				type = "int";
				break;
			}case 2:
			{
				type = "double";
				break;
			}case 3:
			{
				type = "float";
				break;
			}
			}
			symboltable.assign(it.name, type, "v", it.level);
		}
		for (auto it1 : funcTable)
		{
			string type;
			switch (it1.returnType)
			{
			case 0:
			{
				type = "void";
				break;
			}
			case 1:
			{
				type = "int";
				break;
			}case 2:
			{
				type = "double";
				break;
			}case 3:
			{
				type = "float";
				break;
			}
			}
			symboltable.assign(it1.name, type, "f", it1.enterPoint);
		}
	
}
	//得到四元式
	IntermediateCode getcode()
	{
		return code;
	}
	//打印符号表
	void getvaltable()
	{
		for (auto id : symboltable.Symbollist)
		{
			cout << id.name << " , " << id.type << " , " << id.cat << " , " << id.addr<<endl;
		}
	}
	//打印函数表
	void getfunctable()
	{
		for (auto id : funcTable)
		{
			string type;
			switch (id.returnType)
			{
			case 0:
			{
				type = "void";
				break;
			}
			case 1:
			{
				type = "int";
				break;
			}case 2:
			{
				type = "double";
				break;
			}case 3:
			{
				type = "float";
				break;
			}
			}
			cout << id.name << " , " << type << " , " << id.enterPoint<<" , "<<"形参类型：";
			if (!id.paramTypes.empty())
			{
				for (auto j : id.paramTypes)
				{
					string type;
					switch (j)
					{
					case 0:
					{
						type = "void";
						break;
					}
					case 1:
					{
						type = "int";
						break;
					}case 2:
					{
						type = "double";
						break;
					}case 3:
					{
						type = "float";
						break;
					}
					}
					cout << type << " ";
				}
			}
			else
			{
				cout << "void";
			}
			cout << endl;
		}
	}
	//打印中间变量表
	void getintertable()
	{
		for (auto id : intervartable.Symbollist)
		{
			cout << id.name << " , " << id.type << " , " << id.cat << " , " << id.addr<<endl;
		}
	}

};