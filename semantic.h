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
    void addQuad(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result); // 添加四元式
    void print(); // 打印四元式

private:
    std::vector<Quad> quads; // 存储四元式的向量
};

class Parser {
public:
    Parser(Scan& scanner); // 构造函数，初始化解析器
    void parse(); // 解析输入
private:
    Scan& scanner; // 词法分析器
    std::vector<std::pair<std::string, int>> tokens; // 标记序列
    size_t currentTokenIndex; // 当前标记索引
    std::pair<std::string, int> currentToken; // 当前标记
    SymbolTable &symbolTable; // 符号表
    IntermediateCode intermediateCode; // 四元式中间代码
    int tempVarCount; // 临时变量计数

    void program(); // 解析程序
    void decls(); // 解析声明
    void decl(); // 解析单个声明
    void stmts(); // 解析语句
    void stmt(); // 解析单个语句
    std::string expr(); // 解析表达式
    std::string term(); // 解析项
    void match(const std::string& expectedType, const std::string& expectedValue = ""); // 匹配当前标记
    std::string getTokenValue(const std::pair<std::string, int>& token); // 获取标记的值
    std::string newTemp(); // 生成新临时变量
};