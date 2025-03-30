#include"analys_LR.h"
int main() {
  
    ifstream in;
    in.open("text.txt");
    Scan scaner(in);
   
    scaner.read();
    cout << "_______________________" << "词法分析结果" << "__________________________" << endl;
    cout << "Token序列：" << endl;
    scaner.print();
    cout << endl;
    cout << "符号表:" << endl;
    scaner.I.print();
    cout << "_______________________" << "语义分析结果" << "__________________________" << endl;
    Parser parser(scaner);
    parser.Judge();
    cout << "填充后的符号表：" << endl;
    parser.getvaltable();
    cout << endl;
    cout << "中间变量表：" << endl;
    parser.getintertable();
    cout << endl;
    cout << "函数表：" << endl;
    parser.getfunctable();
    cout << endl;
    IntermediateCode code = parser.getcode();
    cout << "原始四元式：" << endl;
    code.output();
    cout << "基本块划分后的四元式："<<endl;
    code.divideBlocks();
    code.outputBlocks();
    cout << "优化后的四元式：" << endl;
    code.optcode();
    code.outputBlocks();
    return 0;
}
