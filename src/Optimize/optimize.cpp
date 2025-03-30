#include "optimize.h"
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <algorithm>

Opt::Opt(std::vector<Block>& blks) : blocks(blks) {}

void Opt::addition(int n, std::string A, std::string typ) {
    int j, k;
    k = getnode(A);
    if (k != 0) {
        for (j = 0; j < maxn; j++) { // �ڽڵ�getnode(A)��ɾ�������
            if (dag.node[k - 1].sign[j].name == A) {
                dag.node[k - 1].sign[j].name = "";
                dag.node[k - 1].sign[j].type = "";
            }
        }
    }
    if (dag.node[n - 1].m_sign.name != "") {
        for (j = 0; j < maxn; j++) { // �ڽڵ�n����Ӹ����
            if (dag.node[n - 1].sign[j].name == "") {
                dag.node[n - 1].sign[j].name = A;
                dag.node[n - 1].sign[j].type = typ;
                break;
            }
        }
    }
    else {
        dag.node[n - 1].m_sign.name = A;
        dag.node[n - 1].m_sign.type = typ;
    }

    if (dag.node[n - 1].m_sign.type == "2") { // �������Ϊ��ʱ�����������Ϊ����ʱ���򻥻�
        for (j = 0; j < maxn; j++) {
            if (dag.node[n - 1].sign[j].type == "1") {
                std::string a;
                a = dag.node[n - 1].sign[j].name;
                dag.node[n - 1].sign[j].name = dag.node[n - 1].m_sign.name;
                dag.node[n - 1].m_sign.name = a;
                dag.node[n - 1].sign[j].type = "2";
                dag.node[n - 1].m_sign.type = "1";
            }
        }
    }
}

int Opt::getnode(std::string B) {
    int n = 0;
    for (int i = 0; i < dag.num; i++) {
        if (dag.node[i].m_sign.name == B) {
            n = i + 1;
        }

        for (int j = 0; j < maxn; j++) {
            if (dag.node[i].sign[j].name == B) {
                n = i + 1;
            }
        }
    }
    return n;
}

void Opt::makeleaf(std::string B, std::string typ, int index) {
    dag.num++;
    dag.node[dag.num - 1].m_sign.name = B;
    dag.node[dag.num - 1].m_sign.type = typ;
    dag.node[dag.num - 1].m_sign.index = index;
    dag.node[dag.num - 1].id = dag.num;
}

void Opt::delnode(int n) {
    dag.node[n - 1].m_sign.name = "";
    dag.node[n - 1].id = 0;
    for (int i = n - 1; i < dag.num - 1; i++) {
        dag.node[i].id = dag.node[i + 1].id;
        dag.node[i].left = dag.node[i + 1].left;
        dag.node[i].m_sign.name = dag.node[i + 1].m_sign.name;
        dag.node[i].m_sign.type = dag.node[i + 1].m_sign.type;
        dag.node[i].op = dag.node[i + 1].op;
        dag.node[i].right = dag.node[i + 1].right;
        for (int j = 0; j < maxn; j++) {
            dag.node[i].sign[j].name = dag.node[i + 1].sign[j].name;
            dag.node[i].sign[j].type = dag.node[i + 1].sign[j].type;
        }
    }
    dag.node[dag.num - 1].id = 0;
    dag.node[dag.num - 1].left = 0;
    dag.node[dag.num - 1].m_sign.name = "";
    dag.node[dag.num - 1].m_sign.type = "";
    dag.node[dag.num - 1].op = "";
    dag.node[dag.num - 1].right = 0;
    for (int j = 0; j < maxn; j++) {
        dag.node[dag.num - 1].sign[j].name = "";
        dag.node[dag.num - 1].sign[j].type = "";
    }
    dag.num--;
}

int Opt::findnode(std::string opl, std::string B, std::string C) {
    int n = 0;
    int l, r, k = 0;
    l = getnode(B);
    r = getnode(C);
    for (int i = 0; i < dag.num; i++) {
        k = 0;
        if (dag.node[i].op == opl) { // op����
            l = dag.node[i].left;
            for (int j = 0; j < maxn; j++) {
                if (dag.node[l - 1].sign[j].name == B) k = 1;
            }
            if ((dag.node[l - 1].m_sign.name == B) || k == 1) { // B����
                k = 0;
                r = dag.node[i].right;
                for (int j = 0; j < maxn; j++) {
                    if (dag.node[r - 1].sign[j].name == C) k = 1;
                }
                if ((dag.node[r - 1].m_sign.name == C) || k == 1) { // C����
                    n = i + 1;
                    break;
                }
            }
        }
    }
    return n;
}

int Opt::makenode(std::string opl, std::string B, std::string C, int index) {
    dag.num++;
    dag.node[dag.num - 1].id = dag.num;
    dag.node[dag.num - 1].left = getnode(B);
    dag.node[dag.num - 1].right = getnode(C);
    dag.node[dag.num - 1].op = opl;
    dag.node[dag.num - 1].m_sign.index = index;
    return dag.num;
}

std::string Opt::coutvalue(std::string opl, std::string B, std::string C) {
    char a;
    char s[maxn];
    float v;
    a = opl[0];
    switch (a) {
    case '+':
        v = atof(B.c_str()) + atof(C.c_str());
        break;
    case '-':
        v = atof(B.c_str()) - atof(C.c_str());
        break;
    case '*':
        v = atof(B.c_str()) * atof(C.c_str());
        break;
    case '/':
        v = atof(B.c_str()) / atof(C.c_str());
        break;
    }
    sprintf_s(s, "%f", v);
    std::string p(s);
    return p;
}

void Opt::makequa(Block& block, std::vector<Quaternary>& optimized_qua) {
    int i, j, k = 0;
    int numB = 0, l, r;
    for (i = 0; i < dag.num; i++) {
        k = 0;
        if (dag.node[i].op == "") { // ��Ҷ�ӽڵ�
            for (j = 0; j < maxn; j++) {
                if (dag.node[i].sign[j].type == "1") {
                    k = 1;
                    numB = j;
                }
            }
            if (dag.node[i].m_sign.type == "3" || (dag.node[i].m_sign.type == "1" && k == 1)) { // �����Ϊ���֣�������з���ʱ����
                
                optimized_qua.push_back({ "=", dag.node[i].sign[numB].name, "", dag.node[i].m_sign.name, dag.node[i].m_sign.index });
            }
        }
        else {
            l = dag.node[i].left;
            r = dag.node[i].right;
  
            optimized_qua.push_back({ dag.node[i].op, dag.node[l - 1].m_sign.name, dag.node[r - 1].m_sign.name, dag.node[i].m_sign.name, dag.node[i].m_sign.index });
        }
    }

    // ���� index �����Ż������Ԫʽ
    std::sort(optimized_qua.begin(), optimized_qua.end(), [](const Quaternary& a, const Quaternary& b) {
        return a.id < b.id;
        });
}

void Opt::deletedag() {
    int i, j;
    for (i = 0; i < dag.num; i++) {
        dag.node[i].id = 0;
        dag.node[i].left = 0;
        dag.node[i].m_sign.name = "";
        dag.node[i].m_sign.type = "";
        dag.node[i].op = "";
        dag.node[i].right = 0;
        for (j = 0; j < maxn; j++) {
            dag.node[i].sign[j].name = "";
            dag.node[i].sign[j].type = "";
        }
    }
    dag.num = 0;
}

void Opt::optimizequa() {
    int newleft, newright; // �½��ڵ�id
    int i = 0, j, k;
    int n;
    std::string p; // ��B,CΪ���֣�p=B op C
    std::string A, B, C; // ����B��C��A��

    for (auto& block : blocks) {
        std::vector<Quaternary> optimized_qua;
        for (auto& qua : block.codes) {
            A = qua.des;
            B = qua.src1;
            C = qua.src2;
            newleft = 0;
            newright = 0;
            if (getnode(B) == 0) {
                makeleaf(B, "1", qua.id);
                newleft = getnode(B);
            }
            switch (qua.op[0]) {
            case '=': // ��Ŀ
                n = getnode(B);
                addition(n, A, "1");
                break;
            case '+':
            case '-':
            case '*':
            case '/': // ˫Ŀ
                if (getnode(C) == 0) {
                    makeleaf(C, "1", qua.id);
                    newright = getnode(C);
                }
                if (isdigit(B[0]) && isdigit(C[0])) { // ������������Ϊ����
                    p = coutvalue(qua.op, B, C);
                    if (newleft != 0) {
                        delnode(newleft);
                    }
                    if (newright != 0) {
                        delnode(newright);
                    }
                    if ((n = getnode(p)) == 0) {
                        makeleaf(p, "3", qua.id);
                        n = getnode(p);
                    }
                }
                else {
                    if ((n = findnode(qua.op, B, C)) == 0) { // ��DAG�в������޷���A=B op C�Ľڵ�
                        n = makenode(qua.op, B, C, qua.id);
                    }
                }
                addition(n, A, "2"); // ��Ӹ��ӱ��
                break;
            default: // �޷��Ż�����Ԫʽֱ����ӵ��Ż�����б���
                optimized_qua.push_back(qua);
                break;
            }
        }
        makequa(block, optimized_qua); // �����Ż�����Ԫʽ
        block.codes = optimized_qua;   // �滻�������е���Ԫʽ
        deletedag(); // ��dag����
    }
}
