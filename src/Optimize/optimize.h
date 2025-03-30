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
        std::string type; // 2为临时变量，3为数字，1为非临时变量
        int index;        // 四元式的索引
    };

    //struct Quaternion {
    //    int block; // 基本块标识
    //    int type;  // 类型-0 1 2 3
    //    std::string op; // 操作符
    //    token num1; // 操作数1
    //    token num2; // 操作数2
    //    token ans; // 结果
    //};

    struct Node { // DAG节点
        int left;
        int right;
        int id;
        std::string op;
        token m_sign; // 主标记
        token sign[maxn]; // 附加标记
    };

    struct DAG { // DAG
        int num = 0;
        Node node[maxn];
    } dag;

    std::vector<Block> &blocks;

    void addition(int n, std::string A, std::string typ); // 把标记A添加到节点n上，顺便删除无用标记
    int getnode(std::string B); // 获取标记为B的节点的id
    void makeleaf(std::string B, std::string typ, int index); // 构造叶子节点
    void delnode(int n); // 删除id为n的节点
    int findnode(std::string opl, std::string B, std::string C); // 在DAG中查找有无符合A=B op C的节点
    int makenode(std::string opl, std::string B, std::string C, int index); // 构造中间节点
    std::string coutvalue(std::string opl, std::string B, std::string C); // 计算两个数运算的值
    void makequa(Block& block, std::vector<Quaternary>& optimized_qua); // 生成block块中优化后的四元式
    void deletedag(); // 对dag清零

public:
    Opt(std::vector<Block>& blks);
    void optimizequa(); // 优化四元式
};
