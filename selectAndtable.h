#pragma once
#include"common.h"
using namespace std;
// 一句语法的定义
struct Rule {
    int id;
    string lhs;                // 左部非终结符
    vector<string> rhs;        // 右部符号串
    bool operator<(const Rule& other) const {
        return tie(lhs, rhs) < tie(other.lhs, other.rhs);
    }
    bool operator==(const Rule& other) const {

        return lhs ==other.lhs&&rhs==other.rhs;
    }
};
// 一个项目
struct Item {
    Rule rule;              // 文法规则
    int dotPosition;        // 圆点位置
    set<string> lookaheads; // 向前看符号集合
    bool operator<(const Item& other) const {
        return tie(rule, dotPosition) < tie(other.rule, other.dotPosition);
    }
    bool operator==(const Item& other) const {

        return rule == other.rule && dotPosition == other.dotPosition && lookaheads == other.lookaheads;
    }
};
// 项目集
struct Term {
    int id=0;                 // 项目集编码
    set<Item> items;        // 项目集内的项
    map<string, int> next;  //用于记录转移
    // 自定义比较运算符
    bool operator<(const Term& other) const {
        return id < other.id;
    }
    bool operator==(const Term& other)const {
        return items == other.items;
    }
};
// LR(1)文法分析类
class GrammarAnalyzerLR {
private:  
    vector<Rule> grammar;   // 文法规则
    vector<Term> termlist;  // 项目集
    set<string> terminals;  // 终结符集合
    set<string> nonTerminals; // 非终结符集合
    string startSymbol;//开始状态
    // 将文法字符串分割
    vector<string> split(const string& s, char delimiter);
    // 检查string中有没有字母
    bool have_alpha(const string& name);
    //计算First集
    set<string> computeFirst(const string& symbol);
    // 计算闭包
    set<Item> closure(set<Item> items);
    //状态转移函数
    set<Item> gotofun(const set<Item>& items, const string& symbol);
    //生成可能产生转移的符号
    set<string> getTransitionSymbols(const Term& term); 
    //生成前缀规约集
    void initialize();
public: 
    map<int, map<string, pair<string,int>>> actionTable;//操作表
    map<int, map<string, int>> gotoTable;//移动表

    //生成操作表
    void generateParsingTable();
    //阅读文法
    void readGrammar(const string& filename);
    //打印操作表
    void print();
    //打印项目集
    void printTermList(ostream& out) const;
    //打印到文件
    void toFile(const string& filename);
    //从文件读取
    void readFromFile(const string& filename);
    //得到文法
    vector<Rule> getgrammar();
};
// LL(1)文法分析类
class GrammarAnalyzerofLL {
private:
    vector<Rule> grammar;                // 文法规则
    map<string, set<string>> firstSets;  // FIRST集
    map<string, set<string>> followSets; // FOLLOW集
    map<Rule, set<string>> selectSets;   // SELECT集
    set<string> terminals;               // 终结符集合
    set<string> nonTerminals;            // 非终结符集合
    string startSymbol;                  // 起始符号

    // 拆分字符串
    vector<string> split(const string& s, char delimiter);

    // 计算FIRST集
    set<string> computeFirst(const string& symbol);

    // 计算FOLLOW集
    void computeFollow();

    // 计算SELECT集
    void computeSelect();

    // 检查是否符合LL(1)文法
    bool isLL1();

    //检查string中有没有字母
    bool have_alpha(const string& name);
public:
    // 读取文法规则
    void readGrammar(const string& filename);

    // 计算FIRST集和FOLLOW集
    void computeFirstAndFollowSets();

    // 打印FIRST集和FOLLOW集
    void printFirstAndFollowSets();

    // 计算并打印SELECT集
    void computeAndPrintSelectSets();

    // 判断是否符合LL(1)文法
    void checkLL1();
    //打印非终结符，终结符
    void print();
};