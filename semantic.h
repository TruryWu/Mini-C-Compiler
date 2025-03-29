#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include"lexical.h"

struct Quad {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;

    Quad(std::string o, std::string a1, std::string a2, std::string res)
        : op(o), arg1(a1), arg2(a2), result(res) {}
};

class IntermediateCode {
public:
    void addQuad(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result); // �����Ԫʽ
    void print(); // ��ӡ��Ԫʽ

private:
    std::vector<Quad> quads; // �洢��Ԫʽ������
};

class Parser {
public:
    Parser(Scan& scanner); // ���캯������ʼ��������
    void parse(); // ��������
private:
    Scan& scanner; // �ʷ�������
    std::vector<std::pair<std::string, int>> tokens; // �������
    size_t currentTokenIndex; // ��ǰ�������
    std::pair<std::string, int> currentToken; // ��ǰ���
    SymbolTable &symbolTable; // ���ű�
    IntermediateCode intermediateCode; // ��Ԫʽ�м����
    int tempVarCount; // ��ʱ��������

    void program(); // ��������
    void decls(); // ��������
    void decl(); // ������������
    void stmts(); // �������
    void stmt(); // �����������
    std::string expr(); // �������ʽ
    std::string term(); // ������
    void match(const std::string& expectedType, const std::string& expectedValue = ""); // ƥ�䵱ǰ���
    std::string getTokenValue(const std::pair<std::string, int>& token); // ��ȡ��ǵ�ֵ
    std::string newTemp(); // ��������ʱ����
};