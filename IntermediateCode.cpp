#include "IntermediateCode.h"
//设置标签
NewLabeler::NewLabeler() {
	index = 1;
}
string NewLabeler::newLabel() {
	return string("Label") + to_string(index++);
}
//划分四元式
void IntermediateCode::divideBlocks() {
    string currentFunction;
    vector<Block> currentBlocks;
    Block currentBlock;
    Block globalBlock;

    map<int, int> blockMap; // 记录每个基本块的起始位置及其在currentBlocks中的索引
    int blockStartIndex = 0;
    int currentIndex = 0;

    for (size_t i = 0; i < code.size(); ++i) {
        Quaternary& q = code[i];

        if (q.op == "function") {
            // 开始处理一个函数块
            if (!currentFunction.empty()) {
                currentBlocks.push_back(currentBlock);
                funcBlocks[currentFunction] = currentBlocks;
            }
            //开始处理新函数
            currentFunction = q.src1;
            currentBlocks.clear();
            currentBlock = Block(nl.newLabel());
            currentBlock.codes.push_back(q);
            blockMap[i] = currentIndex++;
            blockStartIndex = i;

        }
        else if (q.op == "function_end") {
            currentBlock.codes.push_back(q);
            currentBlocks.push_back(currentBlock);
            funcBlocks[currentFunction] = currentBlocks;
            currentFunction.clear();

        }
        else if (currentFunction.empty()) {
            // 处理全局变量
            globalBlock.codes.push_back(q);

        }
        else {
            currentBlock.codes.push_back(q);

            // 处理跳转
            if (q.op == "if_start" || q.op == "else" || q.op == "goto" || q.op == "do" || q.op == "while") {
                currentBlocks.push_back(currentBlock);
                blockMap[blockStartIndex] = currentIndex++;
                currentBlock = Block(nl.newLabel());
                blockStartIndex = i + 1; // 下一个基本块的开始位置
            }
        }
    }

    // 处理最后一个函数的块
    if (!currentFunction.empty()) {
        currentBlocks.push_back(currentBlock);
        funcBlocks[currentFunction] = currentBlocks;
    }

    // 插入全局块
    if (!globalBlock.codes.empty()) {
        funcBlocks["global"] = { globalBlock };
    }

    // 建立基本块之间的连接
    for (auto& func : funcBlocks) {
        vector<Block>& blocks = func.second;
        for (size_t i = 0; i < blocks.size(); ++i) {
            Block& block = blocks[i];
            Quaternary& lastInstr = block.codes.back();
            if (lastInstr.op == "if_start" || lastInstr.op == "else" || lastInstr.op == "goto") {
                int targetIndex = stoi(lastInstr.des);
                if (blockMap.count(targetIndex)) {
                    block.setNext(blockMap[targetIndex]);
                }
                if (lastInstr.op == "if_start" || lastInstr.op == "else") {
                    if (i + 1 < blocks.size()) {
                        block.setNext(block.next1, i + 1);
                    }
                }
            }
            else {
                if (i + 1 < blocks.size()) {
                    block.setNext(i + 1);
                }
            }
        }
    }
}
//输出四元式
void IntermediateCode::output(ostream& out) {
	int i = 0;
	for (vector<Quaternary>::iterator iter = code.begin(); iter != code.end(); iter++, i++) {
		out << setw(4) << i;
		out << "( " << iter->op << " , ";
		out << iter->src1 << " , ";
		out << iter->src2 << " , ";
		out << iter->des << " )";
		out << endl;
	}
}
//输出基本块
void IntermediateCode::outputBlocks(ostream& out) {
	for (map<string, vector<Block> >::iterator iter = funcBlocks.begin(); iter != funcBlocks.end(); iter++) {
		out << "[" << iter->first << "]" << endl;
		for (vector<Block>::iterator bIter = iter->second.begin(); bIter != iter->second.end(); bIter++) {
			out << bIter->name << ":" << endl;
			for (vector<Quaternary>::iterator cIter = bIter->codes.begin(); cIter != bIter->codes.end(); cIter++) {
				out << setw(4) << cIter->id << "(" << cIter->op << "," << cIter->src1 << "," << cIter->src2 << "," << cIter->des << ")" << endl;
			}
			//out << "    " << "next1 = " << bIter->next1 << endl;
			//out << "    " << "next2 = " << bIter->next2 << endl;
		}
		cout << endl;
	}
}
//压入中间代码
void IntermediateCode::emit(Quaternary q) {
	code.push_back(q);
}
//产生四元式
void IntermediateCode::emit(string op, string src1, string src2, string des) {
	emit(Quaternary{ op,src1,src2,des });
}
//回填跳转地址
void IntermediateCode::back_patch(int index, const string& result) {
	if (index >= 0 && index < code.size()) {
		code[index].des = result;
	}
}
//输出
void IntermediateCode::output() {
	output(cout);
}
//打印到文件
void IntermediateCode::output(const char* fileName) {
	ofstream fout;
	fout.open(fileName);
	if (!fout.is_open()) {
		cerr << "file " << fileName << " open error" << endl;
		return;
	}
	output(fout);

	fout.close();
}
//打印基本块
void IntermediateCode::outputBlocks() {
	outputBlocks(cout);
}
//把基本块打印到文件
void IntermediateCode::outputBlocks(const char* fileName) {
	ofstream fout;
	fout.open(fileName);
	if (!fout.is_open()) {
		cerr << "file " << fileName << " open error" << endl;
		return;
	}
	outputBlocks(fout);

	fout.close();
}
//得到下一个的四元式的标签号
int IntermediateCode::nextQuad() {
	return code.size();
}
//设置id
void IntermediateCode::setid()
{
    int i = 0;
    for (auto& it : code)
    {
        it.id = i;
        i++;
    }
}
