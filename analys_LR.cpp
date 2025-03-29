#include"analys_LR.h"
bool operator ==(const SemanticValue& one, const SemanticValue& other) {
	return one.content == other.content;
}

bool operator <(const SemanticValue& one, const SemanticValue& other) {
	return one.content < other.content;
}

SemanticValue::SemanticValue(const SemanticValue& sym) {
	this->content = sym.content;
	this->isVt = sym.isVt;
}

SemanticValue::SemanticValue(const bool& isVt, const string& content) {
	this->isVt = isVt;
	this->content = content;
}

NewTemper::NewTemper() {
	now = 0;
}

string NewTemper::newTemp() {
	return string("T") + to_string(now++);
}

SemanticValue::SemanticValue() {}
Type::Type(const SemanticValue& sym, const string& type) : SemanticValue(sym)
{
	this->type = type;
}

Id::Id(const SemanticValue& sym, const string& name) : SemanticValue(sym) {
	this->name = name;
}

Num::Num(const SemanticValue& sym, const string& number) : SemanticValue(sym) {
	this->number = number;
}

FunctionDeclare::FunctionDeclare(const SemanticValue& sym) : SemanticValue(sym) {}

Parameter::Parameter(const SemanticValue& sym) : SemanticValue(sym) {}

ParameterList::ParameterList(const SemanticValue& sym) : SemanticValue(sym) {}

SentenceBlock::SentenceBlock(const SemanticValue& sym) : SemanticValue(sym) {}

SentenceList::SentenceList(const SemanticValue& sym) : SemanticValue(sym) {}

Sentence::Sentence(const SemanticValue& sym) : SemanticValue(sym) {}

WhileSentence::WhileSentence(const SemanticValue& sym) : SemanticValue(sym) {}

IfSentence::IfSentence(const SemanticValue& sym) : SemanticValue(sym) {}

Expression::Expression(const SemanticValue& sym) : SemanticValue(sym) {}

M::M(const SemanticValue& sym) : SemanticValue(sym) {}

N::N(const SemanticValue& sym) : SemanticValue(sym) {}

AddExpression::AddExpression(const SemanticValue& sym) : SemanticValue(sym) {}

Nomial::Nomial(const SemanticValue& sym) : SemanticValue(sym) {}

Factor::Factor(const SemanticValue& sym) : SemanticValue(sym) {}

ArgumentList::ArgumentList(const SemanticValue& sym) : SemanticValue(sym) {}

Func* Parser::lookUpFunc(string ID) {
	for (vector<Func>::iterator iter = funcTable.begin(); iter != funcTable.end(); iter++) {
		if (iter->name == ID) {
			return &(*iter);
		}
	}
	return NULL;
}

Var* Parser::lookUpVar(string ID) {
	for (vector<Var>::reverse_iterator iter = varTable.rbegin(); iter != varTable.rend(); iter++) {
		if (iter->name == ID) {
			return &(*iter);
		}
	}
	return NULL;
}

SemanticValue* Parser::popSymbol() {
	SemanticValue* ret = symStack.top();
	symStack.pop();
	staStack.pop();
	return ret;
}

void Parser::pushSymbol(SemanticValue* sym) {
	symStack.push(sym);
	if (cheakTable(staStack.top(), sym->content, gotoTable) == -1) {
		outputError(string("语法错误：第") + to_string(lineCount) + "行，不期待的符号" + sym->content);
	}
	auto  bh = cheakTable(staStack.top(), sym->content, gotoTable);
	staStack.push(bh);
}

bool Parser::march(list<string>& argument_list, list<DType>& parameter_list) {
	if (argument_list.size() != parameter_list.size()) {
		return false;
	}
	else {
		return true;
	}
}

