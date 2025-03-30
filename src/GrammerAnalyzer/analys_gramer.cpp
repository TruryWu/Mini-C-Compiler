#include"selectAndtable.h"
//���ķ��ַ����ָ�
vector<string> GrammarAnalyzerofLL:: split(const string& s, char delimiter) {
    vector<string> tokens;          // ���ڴ洢�ָ����ַ���
    string token;                   // ��ʱ�洢�����ַ���
    istringstream tokenStream(s);   // ʹ���ַ��������зָ�
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);    // ���ָ����ַ����洢��tokens��
    }
    return tokens;                  // ���طָ����ַ�������
}

//����First��
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

//����Follow��
void  GrammarAnalyzerofLL:: computeFollow() {
    followSets[startSymbol].insert("$");  // ��ʼ���ŵ�FOLLOW������$
    bool changed = true;                  // ��־FOLLOW���Ƿ����仯
    while (changed) {
        changed = false;
        for (auto& rule : grammar) {
            for (size_t i = 0; i < rule.rhs.size(); ++i) {
                if (nonTerminals.count(rule.rhs[i])) {  // ����Ƿ��ս��
                    set<string> follow;
                    if (i + 1 < rule.rhs.size()) {      // �Ҳ����з���
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
                        changed = true;  // FOLLOW�������仯
                    }
                }
            }
        }
    }
}

// ����SELECT��
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

// ����Ƿ����LL(1)�ķ�
bool GrammarAnalyzerofLL::isLL1() {
    map<string, set<string>> selectUnion;  // ���ڴ洢ÿ�����ս����SELECT���Ĳ���
    for (auto& rule : grammar) {
        for (const string& symbol : selectSets[rule]) {
            if (selectUnion[rule.lhs].count(symbol)) {
                return false;  // ������ظ���˵������LL(1)�ķ�
            }
            selectUnion[rule.lhs].insert(symbol);
        }
    }
    return true;
}

//���string����û����ĸ
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

// ��ȡ�ķ�����
void GrammarAnalyzerofLL::readGrammar(const string& filename) {
    ifstream infile(filename);       // ���ļ�
    string line;
    while (getline(infile, line)) {  // ���ж�ȡ�ļ�����
        vector<string> parts = split(line, ' ');
        Rule rule;
        rule.lhs = parts[0];
        rule.rhs.insert(rule.rhs.end(), parts.begin() + 2, parts.end());
        grammar.push_back(rule);     // ��ӹ����ķ�
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
    startSymbol = grammar[0].lhs;  // �����һ�������LHS����ʼ����
}

// ����FIRST����FOLLOW��
void GrammarAnalyzerofLL::computeFirstAndFollowSets() {
    for (const string& nonTerminal : nonTerminals) {
        computeFirst(nonTerminal);  // �������з��ս����FIRST��
    }
    computeFollow();  // ����FOLLOW��
}

// ��ӡFIRST����FOLLOW��
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

// ���㲢��ӡSELECT��
void GrammarAnalyzerofLL::computeAndPrintSelectSets() {
    computeSelect();  // ����SELECT��
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

// �ж��Ƿ����LL(1)�ķ�
void GrammarAnalyzerofLL::checkLL1() {
    if (isLL1()) {
        cout << "The grammar is LL(1)." << endl;
    }
    else {
        cout << "The grammar is not LL(1)." << endl;
    }
}

//��ӡ���ս�����ս��
void GrammarAnalyzerofLL::print()
{
    cout << "�ս��" << endl;
    for (const auto& x : terminals)
    {
        cout << x << " ";
    }
    cout << endl;
    cout << "���ս��" << endl;
    for (const auto& x : nonTerminals)
    {
        cout << x << " ";
    }
    cout << endl;
}

// ���ķ��ַ����ָ�
vector<string> GrammarAnalyzerLR:: split(const string& s, char delimiter) {
    vector<string> tokens;          // ���ڴ洢�ָ����ַ���
    string token;                   // ��ʱ�洢�����ַ���
    istringstream tokenStream(s);   // ʹ���ַ��������зָ�
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);    // ���ָ����ַ����洢��tokens��
    }
    return tokens;                  // ���طָ����ַ�������
}

// ���string����û����ĸ
bool GrammarAnalyzerLR::have_alpha(const string& name) {
    for (const auto& ch : name) {
        if (isalpha(ch)) {
            return true;
        }
    }
    return false;
}

//����ǰհ��
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

// ����հ�
set<Item> GrammarAnalyzerLR::closure(set<Item> items) {
    if (items.empty()) {
        return {}; // ���ؿ�
    }

    bool updated = true;
    while (updated) {
        updated = false;
        set<Item> newItems; // ����Ŀ��

        for (const auto& item : items) {
            if (item.dotPosition < item.rule.rhs.size()) {
                string symbol = item.rule.rhs[item.dotPosition]; // �õ���ǰ������

                if (nonTerminals.count(symbol)) { // �Է��ս������հ�
                    for (const auto& rule : grammar) {
                        if (rule.lhs == symbol) {
                            Item newItem = { rule, 0, {} };
                            set<string> lookaheads;

                            // ���� lookahead ��
                            if (item.dotPosition + 1 == item.rule.rhs.size()) {
                                lookaheads = item.lookaheads; // �����ǰհ���Ͳ������������ͬ
                            }
                            else {
                                auto nextSymbolFirst = computeFirst(item.rule.rhs[item.dotPosition + 1]);
                                lookaheads.insert(nextSymbolFirst.begin(), nextSymbolFirst.end());

                                int i = 2;
                                while (nextSymbolFirst.count("@") > 0) { // �����һ���ǿղ���
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

                            // �ϲ�����Ŀ��
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

        // �ϲ� items �� newItems
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

//״̬ת�ƺ���
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

//���ɿ��ܲ���ת�Ƶķ���
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

//����ǰ׺��Լ��
void GrammarAnalyzerLR::initialize()
{
    Rule augmentedRule = { 0,"<S'>", {startSymbol} };
    grammar.insert(grammar.begin(), augmentedRule);
    Item startItem = { augmentedRule, 0, {"$"} };
    set<Item> startItems = { startItem };
    Term startTerm = { 0, closure(startItems) };
    termlist.push_back(startTerm);
    // ����ǰ׺��Լ��
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

//�õ��ķ�
vector<Rule> GrammarAnalyzerLR::getgrammar()
{
    readGrammar("grammar.txt");
    Rule augmentedRule = { 0,"<S'>", {startSymbol} };
    grammar.insert(grammar.begin(), augmentedRule);
    return grammar;
}

//���ɲ�����
void GrammarAnalyzerLR::generateParsingTable()
{
    initialize();
    for (const auto& term : termlist) {
        int state = term.id;

        // ������Ŀ���е�ÿ����Ŀ
        for (const auto& item : term.items) {
            if (item.dotPosition < item.rule.rhs.size()) {
                // ��ȡԲ���ķ���
                string symbol = item.rule.rhs[item.dotPosition];

                // ����������ս����������ƽ�����
                if (terminals.count(symbol)&&symbol!="@") {

                    int nextState = term.next.at(symbol);
                    actionTable[state][symbol] = make_pair("s " , nextState);
                }
                if (terminals.count(symbol) && symbol == "@")
                {
                    // ��Լ����
                    for (const auto& lookahead : item.lookaheads) {
                        actionTable[state][lookahead] = make_pair("r " ,item.rule.id);
                    }
                }
            }
            else {
                // �����Ŀ�ǹ�Լ��Ŀ
                if (item.rule.lhs == "<S'>"){
                    // ����״̬
                    actionTable[state]["$"] = make_pair("acc ",0);
                }
                else {
                    // ��Լ����
                    for (const auto& lookahead : item.lookaheads) {
                        actionTable[state][lookahead] = make_pair("r " , item.rule.id);
                    }
                }
            }
        }
        // ������Ŀ����ת�����
        for (const auto& x : term.next) {
            if (nonTerminals.count(x.first)) {
                gotoTable[state][x.first] = x.second;
            }
        }
    }
}

//�Ķ��ķ�
void GrammarAnalyzerLR::readGrammar(const string& filename) {
    ifstream infile(filename);       // ���ļ�
    string line;
    while (getline(infile, line)) {  // ���ж�ȡ�ļ�����
        vector<string> parts = split(line, ' ');
        Rule rule;
        rule.id = grammar.size() + 1;
        rule.lhs = parts[0];
        rule.rhs.insert(rule.rhs.end(), parts.begin() + 2, parts.end());
        grammar.push_back(rule);     // ��ӹ����ķ�
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
    startSymbol = grammar[0].lhs;  // �����һ�������LHS����ʼ����
}

//��ӡ������
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

//��ӡ��Ŀ��
void GrammarAnalyzerLR::printTermList(ostream& out) const {
    out << "Term List:" << endl;

    for (const auto& term : termlist) {
        out << "Term ID: " << term.id << endl;
        out << "Items:" << endl;

        for (const auto& item : term.items) {
            out << "  " << item.rule.lhs << " -> ";
            for (size_t i = 0; i < item.rule.rhs.size(); ++i) {
                if (i == item.dotPosition) {
                    out << "�� ";
                }
                out << item.rule.rhs[i] << " ";
            }
            if (item.dotPosition == item.rule.rhs.size()) {
                out << "��";
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

//��ӡ������goto���ļ�
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

// ���ļ���ȡ������
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