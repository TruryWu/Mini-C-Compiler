#include"analys_LR.h"
int main() {
  
    ifstream in;
    in.open("text.txt");
    Scan scaner(in);
   
    scaner.read();
    cout << "_______________________" << "�ʷ��������" << "__________________________" << endl;
    cout << "Token���У�" << endl;
    scaner.print();
    cout << endl;
    cout << "���ű�:" << endl;
    scaner.I.print();
    cout << "_______________________" << "����������" << "__________________________" << endl;
    Parser parser(scaner);
    parser.Judge();
    cout << "����ķ��ű�" << endl;
    parser.getvaltable();
    cout << endl;
    cout << "�м������" << endl;
    parser.getintertable();
    cout << endl;
    cout << "������" << endl;
    parser.getfunctable();
    cout << endl;
    IntermediateCode code = parser.getcode();
    cout << "ԭʼ��Ԫʽ��" << endl;
    code.output();
    cout << "�����黮�ֺ����Ԫʽ��"<<endl;
    code.divideBlocks();
    code.outputBlocks();
    cout << "�Ż������Ԫʽ��" << endl;
    code.optcode();
    code.outputBlocks();
    return 0;
}
