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
//���캯��
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
//��������
void Parser::program() {
    while (currentTokenIndex < tokens.size())
    {
    decls();
    stmts();
    }
   
}
//��������
void Parser::decls() {
    // ������������ "int a;"
    while (currentToken.first == "K" && scanner.K[currentToken.second-1].second == "int") {
        decl();
    }
}
//������������
void Parser::decl() {
    match("K", "int");  // ƥ��"int"�ؼ���
    std::string id = scanner.I[currentToken.second-1].name;
    match("I");  // ƥ���ʶ��
    symbolTable.declare(id,"m_int","v");
    match("P", ";");  // ƥ��ֺ�
    intermediateCode.addQuad("int", id, "_", "_");  // ����������Ԫʽ
}
//�������
void Parser::stmts() {
    // ������䣬�� "a = 5;" �� "b = a + 10;"
    while (currentToken.first == "I") {
        stmt();
    }
}
//�����������
void Parser::stmt() {
    std::string id = scanner.I[currentToken.second-1].name;
    match("I");  // ƥ���ʶ��
    match("P", "=");  // ƥ��Ⱥ�
    std::string exprResult = expr();
    symbolTable.assign(id, isNumber(exprResult)?std::stoi(exprResult):0);
    intermediateCode.addQuad("=", exprResult, "_", id);  // ���ɸ�ֵ��Ԫʽ
    match("P", ";");  // ƥ��ֺ�
}
//�������ʽ
std::string Parser::expr() {
    std::string termResult = term();
    while (currentToken.first == "P" && scanner.P[currentToken.second-1].second == "+") {
        match("P", "+");  // ƥ��Ӻ�
        std::string termResult2 = term();
        std::string tempVar = newTemp();
        intermediateCode.addQuad("+", termResult, termResult2, tempVar);  // ���ɼӷ���Ԫʽ
        termResult = tempVar;
    }
    return termResult;
}
//������
std::string Parser::term() {
    if (currentToken.first == "I") {  // �����ʶ��
        std::string id = scanner.I[currentToken.second-1].name;
        match("I");
        return id;
    }
    else if (currentToken.first == "C1") {  // ��������
        std::string value = scanner.C1[currentToken.second-1].second;
        match("C1");
        return value;
    }
    else {
        throw std::runtime_error("Unexpected token: " + currentToken.first);
    }
}
//ƥ�䵱ǰ��
void Parser::match(const std::string& expectedType, const std::string& expectedValue) {
    // ��鵱ǰ����Ƿ�ƥ�����������ͺ�ֵ
    if (currentToken.first == expectedType && (expectedValue.empty() || getTokenValue(currentToken) == expectedValue)) {
        currentTokenIndex++;
        if (currentTokenIndex < tokens.size()) {
            currentToken = tokens[currentTokenIndex];
        }
        else {
            currentToken = { "", -1 };  // �������
        }
    }
    else {
        throw std::runtime_error("Unexpected token: " + currentToken.first + " " + getTokenValue(currentToken));
    }
}
//��ȡ���ֵ
std::string Parser::getTokenValue(const std::pair<std::string, int>& token) {
    if (token.first == "K") return scanner.K[token.second-1].second;
    if (token.first == "I") return scanner.I[token.second-1].name;
    if (token.first == "C1") return scanner.C1[token.second-1].second;
    if (token.first == "P") return scanner.P[token.second-1].second;
    return "";
}
//������ʱ����
std::string Parser::newTemp() {
    // ��������ʱ������
    return "t" + std::to_string(tempVarCount++);
}
