#include"selectAndtable.h"
//将文法字符串分割
vector<string> GrammarAnalyzerofLL:: split(const string& s, char delimiter) {
    vector<string> tokens;          // 用于存储分割后的字符串
    string token;                   // 临时存储单个字符串
    istringstream tokenStream(s);   // 使用字符串流进行分割
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);    // 将分割后的字符串存储到tokens中
    }
    return tokens;                  // 返回分割后的字符串集合
}

//计算First集
set<string> GrammarAnalyzerofLL::computeFirst(const string& symbol) {
    if (terminals.count(symbol)) {
        return { symbol };
    }
    if (firstSets.count(symbol)) {
        return firstSets[symbol];
    }
    set<string> result;
    for (auto& rule : grammar) {
        if (rule.lhs == symbol) {
            for (auto sym : rule.rhs) {
                set<string> first = computeFirst(sym);
                if (first.count("@"))
                {
                    first.erase("@");
                    continue;
                }
                result.insert(first.begin(), first.end());
                if (first.count("") == 0) break;
            }
        }
    }
    return firstSets[symbol] = result;
}

//计算Follow集
void  GrammarAnalyzerofLL:: computeFollow() {
    followSets[startSymbol].insert("$");  // 起始符号的FOLLOW集包含$
    bool changed = true;                  // 标志FOLLOW集是否发生变化
    while (changed) {
        changed = false;
        for (auto& rule : grammar) {
            for (size_t i = 0; i < rule.rhs.size(); ++i) {
                if (nonTerminals.count(rule.rhs[i])) {  // 如果是非终结符
                    set<string> follow;
                    if (i + 1 < rule.rhs.size()) {      // 右部还有符号
                        set<string> first = computeFirst(rule.rhs[i + 1]);
                        follow.insert(first.begin(), first.end());
                        follow.erase("@");
                    }
                    if (i + 1 == rule.rhs.size() || firstSets[rule.rhs[i + 1]].count("@")) {
                        follow.insert(followSets[rule.lhs].begin(), followSets[rule.lhs].end());
                    }
                    size_t beforeSize = followSets[rule.rhs[i]].size();
                    followSets[rule.rhs[i]].insert(follow.begin(), follow.end());
                    if (followSets[rule.rhs[i]].size() != beforeSize) {
                        changed = true;  // FOLLOW集发生变化
                    }
                }
            }
        }
    }
}

// 计算SELECT集
void GrammarAnalyzerofLL::computeSelect() {
    for (auto rule : grammar) {
        set<string> first = computeFirst(rule.rhs[0]);
        
        if (first.count("@")) {
            first.erase("@");
            computeFollow();
            first.insert(followSets[rule.lhs].begin(), followSets[rule.lhs].end());
        }
        selectSets[rule] = first;
    }
}

// 检查是否符合LL(1)文法
bool GrammarAnalyzerofLL::isLL1() {
    map<string, set<string>> selectUnion;  // 用于存储每个非终结符的SELECT集的并集
    for (auto& rule : grammar) {
        for (const string& symbol : selectSets[rule]) {
            if (selectUnion[rule.lhs].count(symbol)) {
                return false;  // 如果有重复，说明不是LL(1)文法
            }
            selectUnion[rule.lhs].insert(symbol);
        }
    }
    return true;
}

//检查string中有没有字母
bool GrammarAnalyzerofLL::have_alpha(const string& name)
{
    for (const auto& ch : name)
    {
        if (isalpha(ch))
        {
            return true;
        }
    }
    return false;
}

// 读取文法规则
void GrammarAnalyzerofLL::readGrammar(const string& filename) {
    ifstream infile(filename);       // 打开文件
    string line;
    while (getline(infile, line)) {  // 按行读取文件内容
        vector<string> parts = split(line, ' ');
        Rule rule;
        rule.lhs = parts[0];
        rule.rhs.insert(rule.rhs.end(), parts.begin() + 2, parts.end());
        grammar.push_back(rule);     // 添加规则到文法
        nonTerminals.insert(rule.lhs);
        for (const auto& symbol : rule.rhs) {
            if (symbol[0] != '<' || !have_alpha(symbol)) {
                terminals.insert(symbol);
            }
            else {

                nonTerminals.insert(symbol);
            }
        }
    }
    startSymbol = grammar[0].lhs;  // 假设第一个规则的LHS是起始符号
}

// 计算FIRST集和FOLLOW集
void GrammarAnalyzerofLL::computeFirstAndFollowSets() {
    for (const string& nonTerminal : nonTerminals) {
        computeFirst(nonTerminal);  // 计算所有非终结符的FIRST集
    }
    computeFollow();  // 计算FOLLOW集
}

// 打印FIRST集和FOLLOW集
void GrammarAnalyzerofLL::printFirstAndFollowSets() {
    cout << "FIRST Sets:" << endl;
    for (const auto& entry : firstSets) {
        cout << entry.first << ": ";
        for (const auto& symbol : entry.second) {
            cout << symbol << " ";
        }
        cout << endl;
    }

    cout << "FOLLOW Sets:" << endl;
    for (const auto& entry : followSets) {
        cout << entry.first << ": ";
        for (const auto& symbol : entry.second) {
            cout << symbol << " ";
        }
        cout << endl;
    }
}

// 计算并打印SELECT集
void GrammarAnalyzerofLL::computeAndPrintSelectSets() {
    computeSelect();  // 计算SELECT集
    cout << "SELECT Sets:" << endl;
    for (const auto& entry : selectSets) {
        cout << entry.first.lhs << " -> ";
        for (const string& symbol : entry.first.rhs) {
            cout << symbol << " ";
        }
        cout << ": ";
        for (const string& symbol : entry.second) {
            cout << symbol << " ";
        }
        cout << endl;
    }
}

// 判断是否符合LL(1)文法
void GrammarAnalyzerofLL::checkLL1() {
    if (isLL1()) {
        cout << "The grammar is LL(1)." << endl;
    }
    else {
        cout << "The grammar is not LL(1)." << endl;
    }
}

//打印非终结符，终结符
void GrammarAnalyzerofLL::print()
{
    cout << "终结符" << endl;
    for (const auto& x : terminals)
    {
        cout << x << " ";
    }
    cout << endl;
    cout << "非终结符" << endl;
    for (const auto& x : nonTerminals)
    {
        cout << x << " ";
    }
    cout << endl;
}

// 将文法字符串分割
vector<string> GrammarAnalyzerLR:: split(const string& s, char delimiter) {
    vector<string> tokens;          // 用于存储分割后的字符串
    string token;                   // 临时存储单个字符串
    istringstream tokenStream(s);   // 使用字符串流进行分割
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);    // 将分割后的字符串存储到tokens中
    }
    return tokens;                  // 返回分割后的字符串集合
}

// 检查string中有没有字母
bool GrammarAnalyzerLR::have_alpha(const string& name) {
    for (const auto& ch : name) {
        if (isalpha(ch)) {
            return true;
        }
    }
    return false;
}

//计算前瞻符
set<string> GrammarAnalyzerLR::computeFirst(const string& symbol) {
    if (terminals.count(symbol)) {
        return {symbol};
    }

    set<string> result;
    for (auto& rule : grammar) {
        if (rule.lhs == symbol) {
            bool allNullable = true;
            for (auto& sym : rule.rhs) {
                set<string> first = computeFirst(sym);
                result.insert(first.begin(), first.end());
                if (first.count("@") == 0) {
                    allNullable = false;
                    break;
                } else {
                    result.erase("@");
                }
            }
            if (allNullable) {
                result.insert("@");
            }
        }
    }
    return result;
}

// 计算闭包
set<Item> GrammarAnalyzerLR::closure(set<Item> items) {
    if (items.empty()) {
        return {}; // 返回空
    }

    bool updated = true;
    while (updated) {
        updated = false;
        set<Item> newItems; // 新项目集

        for (const auto& item : items) {
            if (item.dotPosition < item.rule.rhs.size()) {
                string symbol = item.rule.rhs[item.dotPosition]; // 得到当前语句符号

                if (nonTerminals.count(symbol)) { // 对非终结符计算闭包
                    for (const auto& rule : grammar) {
                        if (rule.lhs == symbol) {
                            Item newItem = { rule, 0, {} };
                            set<string> lookaheads;

                            // 计算 lookahead 集
                            if (item.dotPosition + 1 == item.rule.rhs.size()) {
                                lookaheads = item.lookaheads; // 该语句前瞻符和产生它的语句相同
                            }
                            else {
                                auto nextSymbolFirst = computeFirst(item.rule.rhs[item.dotPosition + 1]);
                                lookaheads.insert(nextSymbolFirst.begin(), nextSymbolFirst.end());

                                int i = 2;
                                while (nextSymbolFirst.count("@") > 0) { // 如果下一个是空产生
                                    lookaheads.erase("@");
                                    if (item.dotPosition + i == item.rule.rhs.size()) {
                                        lookaheads.insert(item.lookaheads.begin(), item.lookaheads.end());
                                        break;
                                    }
                                    else {
                                        nextSymbolFirst = computeFirst(item.rule.rhs[item.dotPosition + i]);
                                        lookaheads.insert(nextSymbolFirst.begin(), nextSymbolFirst.end());
                                        i++;
                                    }
                                }
                            }

                            newItem.lookaheads = lookaheads;

                            // 合并新项目集
                            auto it = newItems.find(newItem);
                            if (it != newItems.end()) {
                                set<string> combinedLookaheads = it->lookaheads;
                                combinedLookaheads.insert(lookaheads.begin(), lookaheads.end());
                                if (combinedLookaheads != it->lookaheads) {
                                    updated = true;
                                    newItems.erase(it);
                                    newItem.lookaheads = combinedLookaheads;
                                    newItems.insert(newItem);
                                }
                            }
                            else if (items.count(newItem) == 0) {
                                newItems.insert(newItem);
                                updated = true;
                            }
                        }
                    }
                }
            }
        }

        // 合并 items 和 newItems
        for (auto newItem : newItems) {
            auto it = items.find(newItem);
            if (it != items.end()) {
                newItem.lookaheads.insert(it->lookaheads.begin(), it->lookaheads.end());
                items.erase(it);
            }
            items.insert(newItem);
        }
    }

    return items;
}

//状态转移函数
set<Item> GrammarAnalyzerLR::gotofun(const set<Item>& items, const string& symbol)
{
    set<Item> gotoItems;
    for (const auto& item : items)
    {
        if (item.dotPosition < item.rule.rhs.size() && item.rule.rhs[item.dotPosition] == symbol)
        {
            Item newItem = item;
            newItem.dotPosition++;
            gotoItems.insert(newItem);
        }
    }
    return closure(gotoItems);
}

//生成可能产生转移的符号
set<string> GrammarAnalyzerLR::getTransitionSymbols(const Term& term)
{
    set<string> transitionSymbols;

    for (const auto& item : term.items) {
        if (item.dotPosition < item.rule.rhs.size()) {
            transitionSymbols.insert(item.rule.rhs[item.dotPosition]);
        }
    }
    transitionSymbols.erase("@");
    return transitionSymbols;
}

//生成前缀规约集
void GrammarAnalyzerLR::initialize()
{
    Rule augmentedRule = { 0,"<S'>", {startSymbol} };
    grammar.insert(grammar.begin(), augmentedRule);
    Item startItem = { augmentedRule, 0, {"$"} };
    set<Item> startItems = { startItem };
    Term startTerm = { 0, closure(startItems) };
    termlist.push_back(startTerm);
    // 生成前缀归约集
    bool updated = true;
    while (updated) {
        updated = false;
        set<Term> newTerms;
        for (auto& term : termlist)
        {
            set<string> symbols = getTransitionSymbols(term);
            for (const auto& symbol : symbols) {
                auto gotoItems = gotofun(term.items, symbol);
                if (!gotoItems.empty()) {
                    Term gototerm;
                    gototerm.items = gotoItems;
                    auto it = find_if(termlist.begin(), termlist.end(),
                        [&](const Term& t) { return t.items == gotoItems; });
                    if (it != termlist.end())
                    {
                        term.next.insert(make_pair(symbol, it->id));
                    }
                    else
                    {
                        gototerm.id = (int)(newTerms.size() + termlist.size());
                        term.next.insert(make_pair(symbol, gototerm.id));
                        newTerms.insert(gototerm);
                        updated = true;
                    }
                }

            }
        }
        termlist.insert(termlist.end(), newTerms.begin(), newTerms.end());
    }
}

//得到文法
vector<Rule> GrammarAnalyzerLR::getgrammar()
{
    readGrammar("grammar.txt");
    Rule augmentedRule = { 0,"<S'>", {startSymbol} };
    grammar.insert(grammar.begin(), augmentedRule);
    return grammar;
}

//生成操作表
void GrammarAnalyzerLR::generateParsingTable()
{
    initialize();
    for (const auto& term : termlist) {
        int state = term.id;

        // 遍历项目集中的每个项目
        for (const auto& item : term.items) {
            if (item.dotPosition < item.rule.rhs.size()) {
                // 获取圆点后的符号
                string symbol = item.rule.rhs[item.dotPosition];

                // 如果符号是终结符，则进行移进操作
                if (terminals.count(symbol)&&symbol!="@") {

                    int nextState = term.next.at(symbol);
                    actionTable[state][symbol] = make_pair("s " , nextState);
                }
                if (terminals.count(symbol) && symbol == "@")
                {
                    // 归约操作
                    for (const auto& lookahead : item.lookaheads) {
                        actionTable[state][lookahead] = make_pair("r " ,item.rule.id);
                    }
                }
            }
            else {
                // 如果项目是归约项目
                if (item.rule.lhs == "<S'>"){
                    // 接受状态
                    actionTable[state]["$"] = make_pair("acc ",0);
                }
                else {
                    // 归约操作
                    for (const auto& lookahead : item.lookaheads) {
                        actionTable[state][lookahead] = make_pair("r " , item.rule.id);
                    }
                }
            }
        }
        // 遍历项目集的转移情况
        for (const auto& x : term.next) {
            if (nonTerminals.count(x.first)) {
                gotoTable[state][x.first] = x.second;
            }
        }
    }
}

//阅读文法
void GrammarAnalyzerLR::readGrammar(const string& filename) {
    ifstream infile(filename);       // 打开文件
    string line;
    while (getline(infile, line)) {  // 按行读取文件内容
        vector<string> parts = split(line, ' ');
        Rule rule;
        rule.id = grammar.size() + 1;
        rule.lhs = parts[0];
        rule.rhs.insert(rule.rhs.end(), parts.begin() + 2, parts.end());
        grammar.push_back(rule);     // 添加规则到文法
        nonTerminals.insert(rule.lhs);
        for (const auto& symbol : rule.rhs) {
            if (symbol[0] != '<' || !have_alpha(symbol)) {
                terminals.insert(symbol);
            }
            else {

                nonTerminals.insert(symbol);
            }
        }
    }
    startSymbol = grammar[0].lhs;  // 假设第一个规则的LHS是起始符号
}

//打印操作表
void GrammarAnalyzerLR::print()
{
    cout << "ACTION Table:" << endl;
    for (const auto& i : actionTable) {
        cout << "State " << i.first << ":" << endl;
        for (const auto& j : i.second) {
            cout << "  " << j.first << " -> " << j.second.first<<j.second.second << endl;
        }
    }
    cout << "GOTO Table:" << endl;
    for (const auto& i : gotoTable) {
        cout << "State " << i.first << ":" << endl;
        for (const auto& j : i.second) {
            cout << "  " << j.first << " -> " << j.second << endl;
        }
    }
}

//打印项目集
void GrammarAnalyzerLR::printTermList(ostream& out) const {
    out << "Term List:" << endl;

    for (const auto& term : termlist) {
        out << "Term ID: " << term.id << endl;
        out << "Items:" << endl;

        for (const auto& item : term.items) {
            out << "  " << item.rule.lhs << " -> ";
            for (size_t i = 0; i < item.rule.rhs.size(); ++i) {
                if (i == item.dotPosition) {
                    out << "· ";
                }
                out << item.rule.rhs[i] << " ";
            }
            if (item.dotPosition == item.rule.rhs.size()) {
                out << "·";
            }
            out << ", Lookaheads: { ";
            for (const auto& lookahead : item.lookaheads) {
                out << lookahead << " ";
            }
            out << "}" << endl;
        }

        out << "Transitions:" << endl;
        for (const auto& index : term.next) {
            out << "  " << index.first << " -> " << index.second << endl;
        }

        out << "----------------------------------------" << endl;
    }
}

//打印分析表，goto表到文件
void GrammarAnalyzerLR::toFile(const string& filename)
{
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    outfile << "ACTION Table:" << endl;
    for (const auto& i : actionTable) {
        outfile << "State " << i.first << ":" << endl;
        for (const auto& j : i.second) {
            outfile << "  " << j.first << " -> " << j.second.first << j.second.second << endl;
        }
    }

    outfile << "GOTO Table:" << endl;
    for (const auto& i : gotoTable) {
        outfile << "State " << i.first << ":" << endl;
        for (const auto& j : i.second) {
            outfile << "  " << j.first << " -> " << j.second << endl;
        }
    }

    outfile.close();
}

// 从文件读取操作表
void GrammarAnalyzerLR::readFromFile(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    bool isActionTable = false;
    bool isGotoTable = false;
    int currentState = -1;

    while (getline(infile, line)) {
        if (line == "ACTION Table:") {
            isActionTable = true;
            isGotoTable = false;
            continue;
        }
        if (line == "GOTO Table:") {
            isActionTable = false;
            isGotoTable = true;
            continue;
        }

        if (line.find("State") != string::npos&& line.find("<") == string::npos) {
            size_t pos = line.find(" ");
            currentState = stoi(line.substr(pos + 1, line.find(":") - pos-1));
            continue;
        }

        if (isActionTable && line.find("->") != string::npos) {
            size_t pos = line.find(" -> ");
            string symbol = line.substr(2, pos - 2);
            string action = line.substr(pos + 4);
            size_t spacePos = action.find(" ");
            string actionType = action.substr(0, spacePos);
            int actionValue = stoi(action.substr(spacePos + 1));
            actionTable[currentState][symbol] = make_pair(actionType, actionValue);
        }

        if (isGotoTable && line.find("->") != string::npos) {
            size_t pos = line.find(" -> ");
            string symbol = line.substr(2, pos - 2);
            int nextState = stoi(line.substr(pos + 4));
            gotoTable[currentState][symbol] = nextState;
        }
    }

    infile.close();
}