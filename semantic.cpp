#include"semantic.h"
bool isNumber(const string& str) {
    return str.find_first_not_of("0123456789") == string::npos;
}
void IntermediateCode::addQuad(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result) {
    quads.emplace_back(op, arg1, arg2, result);
}

void IntermediateCode::print() {
    for (const auto& quad : quads) {
        std::cout << "(" << quad.op << ", " << quad.arg1 << ", " << quad.arg2 << ", " << quad.result << ")\n";
    }
}
//构造函数
Parser::Parser(Scan& scanner) : scanner(scanner), currentTokenIndex(0), tempVarCount(0) , symbolTable(scanner.I){
    tokens = scanner.Token;
    if (!tokens.empty()) {
        currentToken = tokens[currentTokenIndex];
    }
}

void Parser::parse() {
    program();
    intermediateCode.print();
}
//解析输入
void Parser::program() {
    while (currentTokenIndex < tokens.size())
    {
    decls();
    stmts();
    }
   
}
//解析声明
void Parser::decls() {
    // 处理声明，如 "int a;"
    while (currentToken.first == "K" && scanner.K[currentToken.second-1].second == "int") {
        decl();
    }
}
//解析单个声明
void Parser::decl() {
    match("K", "int");  // 匹配"int"关键字
    std::string id = scanner.I[currentToken.second-1].name;
    match("I");  // 匹配标识符
    symbolTable.declare(id,"m_int","v");
    match("P", ";");  // 匹配分号
    intermediateCode.addQuad("int", id, "_", "_");  // 生成声明四元式
}
//解析语句
void Parser::stmts() {
    // 处理语句，如 "a = 5;" 或 "b = a + 10;"
    while (currentToken.first == "I") {
        stmt();
    }
}
//解析单个语句
void Parser::stmt() {
    std::string id = scanner.I[currentToken.second-1].name;
    match("I");  // 匹配标识符
    match("P", "=");  // 匹配等号
    std::string exprResult = expr();
    symbolTable.assign(id, isNumber(exprResult)?std::stoi(exprResult):0);
    intermediateCode.addQuad("=", exprResult, "_", id);  // 生成赋值四元式
    match("P", ";");  // 匹配分号
}
//解析表达式
std::string Parser::expr() {
    std::string termResult = term();
    while (currentToken.first == "P" && scanner.P[currentToken.second-1].second == "+") {
        match("P", "+");  // 匹配加号
        std::string termResult2 = term();
        std::string tempVar = newTemp();
        intermediateCode.addQuad("+", termResult, termResult2, tempVar);  // 生成加法四元式
        termResult = tempVar;
    }
    return termResult;
}
//解析项
std::string Parser::term() {
    if (currentToken.first == "I") {  // 处理标识符
        std::string id = scanner.I[currentToken.second-1].name;
        match("I");
        return id;
    }
    else if (currentToken.first == "C1") {  // 处理常整数
        std::string value = scanner.C1[currentToken.second-1].second;
        match("C1");
        return value;
    }
    else {
        throw std::runtime_error("Unexpected token: " + currentToken.first);
    }
}
//匹配当前项
void Parser::match(const std::string& expectedType, const std::string& expectedValue) {
    // 检查当前标记是否匹配期望的类型和值
    if (currentToken.first == expectedType && (expectedValue.empty() || getTokenValue(currentToken) == expectedValue)) {
        currentTokenIndex++;
        if (currentTokenIndex < tokens.size()) {
            currentToken = tokens[currentTokenIndex];
        }
        else {
            currentToken = { "", -1 };  // 结束标记
        }
    }
    else {
        throw std::runtime_error("Unexpected token: " + currentToken.first + " " + getTokenValue(currentToken));
    }
}
//获取标记值
std::string Parser::getTokenValue(const std::pair<std::string, int>& token) {
    if (token.first == "K") return scanner.K[token.second-1].second;
    if (token.first == "I") return scanner.I[token.second-1].name;
    if (token.first == "C1") return scanner.C1[token.second-1].second;
    if (token.first == "P") return scanner.P[token.second-1].second;
    return "";
}
//生成临时变量
std::string Parser::newTemp() {
    // 生成新临时变量名
    return "t" + std::to_string(tempVarCount++);
}
