#include "lexical.h"
char Scan::getNextChar()
{
    char c;
    while (input.get(c)) {

        if (c == '\n') {
            lineCount++;
            return '\n';
        }
        else
        {
            break;
        }
    }
    if (input.eof())
        return 0;
    else
        return c;
}

void Scan::read() {

    Constants constants;
    constants.x = cur_str;

    cur_char = getNextChar();

    while (true) {

        cur_str = "";
        //if (input.eof()) break;
        //cur_char = getNextChar();
        if (cur_char == '\n') {
            Token.push_back(make_pair("NEXTLINE", 0));
            if (input.eof()) break;
            cur_char = getNextChar();
        }
        if (isalpha(cur_char)) {
            while (isalnum(cur_char)) {
                cur_str += cur_char;
                if (input.eof()) break;
                cur_char = getNextChar();

            }
            anal_type();
            insert();
        }
        else if (isdigit(cur_char) || (cur_char == '0' && (input.peek() == 'x' || input.peek() == 'X'))) {
            if (cur_char == '0' && (input.peek() == 'x' || input.peek() == 'X')) {
                // 处理十六进制数
                cur_str += cur_char;


                if (input.eof()) break;
                cur_char = getNextChar(); // 加上 'x' 或 'X'
                cur_str += cur_char;



                if (input.eof()) break;
                cur_char = getNextChar();



                while (isxdigit(cur_char)) {
                    cur_str += cur_char;
                    if (input.eof()) break;
                    cur_char = getNextChar();



                }
                Constants num(cur_str);
                constants.x = cur_str;
                int idf = num.validateString();

                if (idf == -1) {
                    cout << "在第" << lineCount << "行出现错误" << endl;
                    return;
                }
                else if (idf == 5)
                {
                    type = 5;
                    if (input.eof()) break;
                    cur_char = getNextChar();
                    insert();
                }
                else
                {
                    type = 6;
                    if (input.eof()) break;
                    cur_char = getNextChar();
                    insert();
                }
            }
            else {
                // 处理普通数字和科学计数法
                bool is_exponent = false;
                while (isdigit(cur_char) || cur_char == '.' || cur_char == 'e' || cur_char == 'E' || (is_exponent && (cur_char == '+' || cur_char == '-'))) {
                    cur_str += cur_char;
                    if (cur_char == 'e' || cur_char == 'E') {
                        is_exponent = true;

                    }
                    else if (is_exponent && (cur_char == '+' || cur_char == '-')) {
                        is_exponent = false;

                    }
                    if (input.eof()) break;
                    constants.x = cur_str;
                    cur_char = getNextChar();
                }
                Constants num(cur_str);
                int idf = num.validateString();

                if (idf == -1) {
                    cout << "在第" << lineCount << "行出现错误" << endl;
                    return;
                }
                else if (idf == 5)
                {
                    type = 5;
                    insert();

                }
                else
                {
                    type = 6;
                    insert();

                }
            }
        }
        else if (cur_char == '+' || cur_char == '-') {
            cur_str += cur_char;
            if (input.eof()) break;
            cur_char = getNextChar();
            type = 2;
            insert();
        }
        else if (cur_char == '*')
        {
            cur_str += cur_char;
            if (input.eof()) break;
            cur_char = getNextChar();
            type = 2;
            insert();
        }


        else if (cur_char == '/') {

            //行注释
            if (input.peek() == '/') {
                input.get();
                string buf;
                getline(input,buf);
                Token.push_back(make_pair("LCOMMENT", 0));
                cur_char = getNextChar();
            }
            //段注释
            else if (input.peek() == '*') {
                input.get();
                string buf = "/*";
                
                while (input >> cur_char) {
                    buf += cur_char;
                    if (cur_char == '*') {
                        input >> cur_char;
                        buf += cur_char;
                        if (cur_char == '/') {


                            Token.push_back(make_pair("PCOMMENT", 0));
                            break;
                        }
                    }
                }
                cur_char = getNextChar();
                
                //读到最后都没找到*/，因不满足while循环条件退出
                if (input.eof()) {
                    cout << "ERROR" << string("词法分析第") + to_string(lineCount) + string("行：段注释没有匹配的*/");
                }
                
            }
            //除法
            else {
                cur_str += cur_char;
                if (input.eof()) break;
                cur_char = getNextChar();
                type = 2;
                insert();
            }
        }


        else if (cur_char == '!')
        {
            cur_str += cur_char;
            if (input.peek() == '=') {
                cur_str += cur_char;
                if (input.eof()) break;
                cur_char = getNextChar();
                if (input.eof()) break;
                cur_char = getNextChar();
                type = 2;
                insert();
            }
            else {
                cout << "ERROR" << string("词法分析第") + to_string(lineCount) + string("行：未知符号！");
            }
        }
        else if (cur_char == '=')
        {
            if (input.peek() == '=') {
                if (input.eof()) break;
                cur_char = getNextChar();
                if (input.eof()) break;
                cur_char = getNextChar();
                cur_str = "==";
            }
            else {
                cur_str = "=";
                if (input.eof()) break;
                cur_char = getNextChar();
            }
            type = 2;
            insert();
        }
        else if (cur_char == '>')
        {
            if (input.peek() == '=') {
                if (input.eof()) break;
                cur_char = getNextChar();
                if (input.eof()) break;
                cur_char = getNextChar();
                cur_str = ">=";
            }
            else {
                cur_str = ">";
                if (input.eof()) break;
                cur_char = getNextChar();
            }
            type = 2;
            insert();
        }
        else if (cur_char == '<')
        {
            if (input.peek() == '=') {
                if (input.eof()) break;
                cur_char = getNextChar();
                if (input.eof()) break;
                cur_char = getNextChar();
                cur_str = "<=";
            }
            else {
                cur_str = "<";
                if (input.eof()) break;
                cur_char = getNextChar();
            }
            type = 2;
            insert();
        }
        else if (isspace(cur_char)) {
            if (input.eof()) break;
            cur_char = getNextChar();
            continue;
        }
        else
        {
            cur_str += cur_char;
            if (input.eof()) break;
            cur_char = getNextChar();
            type = 2;
            insert();
        }

    }
}

void Scan::anal_type() {
    for (int j = 0; j != K.size(); j++) {
        if (K[j].second == cur_str) {
            type = 1; // 关键字
            return;
        }
    }
    type = 0; // 标识符
}

int Scan::isinit(SymbolTable& name)
{
    for (int j = 0; j < name.size(); j++) {
        if (name[j].name == cur_str) {
            return j;
        }
    }
    return -1;
}

int Scan::isinit(vector<pair<int, string>>& name) {
    for (int j = 0; j < name.size(); j++) {
        if (name[j].second == cur_str) {
            return j;
        }
    }
    return -1;
}

bool Scan::insert() {
    switch (type) {
    case 0: { // 标识符
        int j = isinit(I);
        if (j == -1) {
            int index = I.creat(cur_str);
            Token.push_back({ "I", index });
        }
        else {
            Token.push_back({ "I", j + 1 });
        }
        break;
    }
    case 1: { // 关键字
        int j = isinit(K);
        if (j != -1) {
            Token.push_back({ "K", K[j].first });
        }
        break;
    }
    case 2: { // 界符
        int j = isinit(P);
        if (j == -1) {
            int index = P.empty() ? 1 : P.back().first + 1;
            P.push_back({ index, cur_str });
            Token.push_back({ "P", index });
        }
        else {
            Token.push_back({ "P", P[j].first });
        }
        break;
    }
    case 3: { // 字符常量
        int index = CT.empty() ? 1 : CT.back().first + 1;
        CT.push_back({ index, cur_str });
        Token.push_back({ "CT", index });
        break;
    }
    case 4:
    {//
        int index = ST.empty() ? 1 : ST.back().first + 1;
        ST.push_back({ index, cur_str });
        Token.push_back({ "ST", index });
        break;
    }
    case 5: { // 常整数
        int index = C1.empty() ? 1 : C1.back().first + 1;
        C1.push_back({ index, cur_str });
        Token.push_back({ "C1", index });
        break;
    }
    case 6:
    {
        int index = C2.empty() ? 1 : C2.back().first + 1;
        C2.push_back({ index, cur_str });
        Token.push_back({ "C2", index });
        break;
    }
    default:
        return false;
    }
    return true;
}

Constants::Constants(string constants) {
    x = constants;
}

void Constants::Conter() {
    validateString();

    if (isHex) {
        Int = stoi(x, nullptr, 16);
        DoubleLeft = 0;
        DoubleRight = 0;
    }
    else if (isOct) {
        Int = stoi(x, nullptr, 8);
        DoubleLeft = 0;
        DoubleRight = 0;
    }
    else if (isBin) {
        Int = stoi(x.substr(2), nullptr, 2);
        DoubleLeft = 0;
        DoubleRight = 0;
    }
    else {
        unsigned int ePos = x.find('e');
        if (ePos != string::npos) {
            double eValue = scientificToDecimal(x);
            DoubleLeft = static_cast<int>(eValue);
            DoubleRight = eValue - DoubleLeft;
            Int = 0;
        }
        else {
            unsigned int dotPos = x.find('.');
            if (dotPos != string::npos) {
                string left = x.substr(0, dotPos);
                string right = x.substr(dotPos + 1);
                DoubleLeft = stoi(left);
                DoubleRight = stod("0." + right);
                Int = 0;
            }
            else {
                Int = stoi(x);
                DoubleLeft = 0;
                DoubleRight = 0;
            }
        }
    }
}

double Constants::scientificToDecimal(string s) {
    unsigned int eIndex = s.find('e');
    double base = stod(s.substr(0, eIndex));
    if (eIndex + 1 >= s.length()) {
        return 0;
        exit(1);
    }
    int exp = stoi(s.substr(eIndex + 1));
    return base * pow(10, exp);
}

int Constants::validateString() {

    if (x.length() > 2 && (x[0] == '0')) {
        if (x[1] == 'x' || x[1] == 'X') {
            for (unsigned int i = 2; i < x.length(); ++i) {
                if (!isxdigit(x[i])) {
                    return -1;
                    exit(1);
                }
            }
            isHex = true;

        }
        else if (x[1] == 'b' || x[1] == 'B') {
            for (unsigned int i = 2; i < x.length(); ++i) {
                if (x[i] != '0' && x[i] != '1' && x[i] != '.' && x[i] != 'e') {
                    return -1;
                    exit(1);
                }
            }
            isBin = true;

        }
        else {
            for (unsigned int i = 1; i < x.length(); ++i) {
                if (x[i] < '0' || x[i] > '7' && x[i] != '.' && x[i] != 'e') {
                    return -1;
                    exit(1);
                }
            }
            isOct = true;

        }

    }
    bool seenDot = false;
    bool seenE = false;
    unsigned int i = 0;
    if (isHex || isBin || isOct) {
        i = i + 2;
    }
    for (; i < x.length(); ++i) {

        if (isdigit(x[i])) {
            continue;
        }
        else if (x[i] == '.') {
            if (seenDot || seenE) {
                return -1;
                exit(1);
            }
            seenDot = true;
        }
        else if (x[i] == 'e') {
            if (seenE) {
                return -1;
                exit(1);
            }
            else if (i + 1 >= x.length()) {
                return -1;
                exit(1);
            }
            seenE = true;
            if (i + 1 < x.length() && (x[i + 1] == '-' || x[i + 1] == '+')) {
                ++i;
                if (i + 1 >= x.length()) {
                    return -1;
                    exit(1);
                }
            }
        }
        else if (x[i] == '-' || x[i] == '+') {
            if (i != 0 && x[i - 1] != 'e') {
                return -1;
                exit(1);
            }
        }
        else if (isHex && isxdigit(x[i])) {
            i++;
        }
        else {
            return -1;
            exit(1);
        }


    }

    if (!isHex && !isBin && !isOct && !seenDot && !seenE) {
        return isconstants;
    }
    return isNum;
}

void Scan::print()
{
    int i = 0;
    for (i; i < Token.size(); i++)
    {
        cout <<"(" << Token[i].first << "," << Token[i].second<<")"<<" ";
    }
}


