#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "common.h"
const int maxn = 100;

class Opt {
private:
    struct token {
        std::string name;
        std::string type; // 2Ϊ��ʱ������3Ϊ���֣�1Ϊ����ʱ����
        int index;        // ��Ԫʽ������
    };

    //struct Quaternion {
    //    int block; // �������ʶ
    //    int type;  // ����-0 1 2 3
    //    std::string op; // ������
    //    token num1; // ������1
    //    token num2; // ������2
    //    token ans; // ���
    //};

    struct Node { // DAG�ڵ�
        int left;
        int right;
        int id;
        std::string op;
        token m_sign; // �����
        token sign[maxn]; // ���ӱ��
    };

    struct DAG { // DAG
        int num = 0;
        Node node[maxn];
    } dag;

    std::vector<Block> &blocks;

    void addition(int n, std::string A, std::string typ); // �ѱ��A��ӵ��ڵ�n�ϣ�˳��ɾ�����ñ��
    int getnode(std::string B); // ��ȡ���ΪB�Ľڵ��id
    void makeleaf(std::string B, std::string typ, int index); // ����Ҷ�ӽڵ�
    void delnode(int n); // ɾ��idΪn�Ľڵ�
    int findnode(std::string opl, std::string B, std::string C); // ��DAG�в������޷���A=B op C�Ľڵ�
    int makenode(std::string opl, std::string B, std::string C, int index); // �����м�ڵ�
    std::string coutvalue(std::string opl, std::string B, std::string C); // ���������������ֵ
    void makequa(Block& block, std::vector<Quaternary>& optimized_qua); // ����block�����Ż������Ԫʽ
    void deletedag(); // ��dag����

public:
    Opt(std::vector<Block>& blks);
    void optimizequa(); // �Ż���Ԫʽ
};
