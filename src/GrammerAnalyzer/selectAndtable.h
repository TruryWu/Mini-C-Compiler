#pragma once
#include"common.h"
using namespace std;
// һ���﷨�Ķ���
struct Rule {
    int id;
    string lhs;                // �󲿷��ս��
    vector<string> rhs;        // �Ҳ����Ŵ�
    bool operator<(const Rule& other) const {
        return tie(lhs, rhs) < tie(other.lhs, other.rhs);
    }
    bool operator==(const Rule& other) const {

        return lhs ==other.lhs&&rhs==other.rhs;
    }
};
// һ����Ŀ
struct Item {
    Rule rule;              // �ķ�����
    int dotPosition;        // Բ��λ��
    set<string> lookaheads; // ��ǰ�����ż���
    bool operator<(const Item& other) const {
        return tie(rule, dotPosition) < tie(other.rule, other.dotPosition);
    }
    bool operator==(const Item& other) const {

        return rule == other.rule && dotPosition == other.dotPosition && lookaheads == other.lookaheads;
    }
};
// ��Ŀ��
struct Term {
    int id=0;                 // ��Ŀ������
    set<Item> items;        // ��Ŀ���ڵ���
    map<string, int> next;  //���ڼ�¼ת��
    // �Զ���Ƚ������
    bool operator<(const Term& other) const {
        return id < other.id;
    }
    bool operator==(const Term& other)const {
        return items == other.items;
    }
};
// LR(1)�ķ�������
class GrammarAnalyzerLR {
private:  
    vector<Rule> grammar;   // �ķ�����
    vector<Term> termlist;  // ��Ŀ��
    set<string> terminals;  // �ս������
    set<string> nonTerminals; // ���ս������
    string startSymbol;//��ʼ״̬
    // ���ķ��ַ����ָ�
    vector<string> split(const string& s, char delimiter);
    // ���string����û����ĸ
    bool have_alpha(const string& name);
    //����First��
    set<string> computeFirst(const string& symbol);
    // ����հ�
    set<Item> closure(set<Item> items);
    //״̬ת�ƺ���
    set<Item> gotofun(const set<Item>& items, const string& symbol);
    //���ɿ��ܲ���ת�Ƶķ���
    set<string> getTransitionSymbols(const Term& term); 
    //����ǰ׺��Լ��
    void initialize();
public: 
    map<int, map<string, pair<string,int>>> actionTable;//������
    map<int, map<string, int>> gotoTable;//�ƶ���

    //���ɲ�����
    void generateParsingTable();
    //�Ķ��ķ�
    void readGrammar(const string& filename);
    //��ӡ������
    void print();
    //��ӡ��Ŀ��
    void printTermList(ostream& out) const;
    //��ӡ���ļ�
    void toFile(const string& filename);
    //���ļ���ȡ
    void readFromFile(const string& filename);
    //�õ��ķ�
    vector<Rule> getgrammar();
};
// LL(1)�ķ�������
class GrammarAnalyzerofLL {
private:
    vector<Rule> grammar;                // �ķ�����
    map<string, set<string>> firstSets;  // FIRST��
    map<string, set<string>> followSets; // FOLLOW��
    map<Rule, set<string>> selectSets;   // SELECT��
    set<string> terminals;               // �ս������
    set<string> nonTerminals;            // ���ս������
    string startSymbol;                  // ��ʼ����

    // ����ַ���
    vector<string> split(const string& s, char delimiter);

    // ����FIRST��
    set<string> computeFirst(const string& symbol);

    // ����FOLLOW��
    void computeFollow();

    // ����SELECT��
    void computeSelect();

    // ����Ƿ����LL(1)�ķ�
    bool isLL1();

    //���string����û����ĸ
    bool have_alpha(const string& name);
public:
    // ��ȡ�ķ�����
    void readGrammar(const string& filename);

    // ����FIRST����FOLLOW��
    void computeFirstAndFollowSets();

    // ��ӡFIRST����FOLLOW��
    void printFirstAndFollowSets();

    // ���㲢��ӡSELECT��
    void computeAndPrintSelectSets();

    // �ж��Ƿ����LL(1)�ķ�
    void checkLL1();
    //��ӡ���ս�����ս��
    void print();
};