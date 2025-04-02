# Mini C Compiler

## 项目简介

Mini C Compiler 是一个简单的 C 语言编译器，完全由 C++ 编写，从零开始实现了 C 语言的编译过程。项目旨在帮助学习编译原理与实现，并提供一个简洁、易于理解的编译器实现示例。
## 文法介绍
Mini C Compiler 实现的编译语言的文法包括C语言风格的变量声明，四则运算，布尔运算，for，while循环，if语句等
## 功能实现

- **词法分析**  
  对 C 语言源代码进行词法分析，生成标记（Token）序列。

- **语法分析&语义分析**  
  通过文法的Action和Goto表进行语法分析，确保源代码的语法正确性同时检查变量声明、作用域以及类型匹配等语义问题。

- **中间代码生成**  
  生成中间表示，以便后续处理。

- **中间代码优化**  
  对生成的中间代码进行DAG优化。

## 项目结构
```
Mini C Compile:
│  ActionAndGoto.txt #Action和Goto表
│  grammar.txt       #Mini C 文法规则
│  tablist.txt       #项目集
│  text.txt          #测试程序
│  textgra.txt       #测试文法
└─src
    │  main.cpp  #主程序，这里打印了每个部分的内容，可根据需求进行修改。
    │  
    ├─GrammerAnalyzer #文法分析部分，通过LL(1) or LR(1)分析对grammar.txt当中的文法规则进行分析生成action和goto表以及项目集(可选)
    │      analys_gramer.cpp
    │      selectAndtable.h
    │      
    ├─IntermediateCode #中间语言部分，本编译器使用四元式作为中间语言，同时通过语义分析生成基本块
    │      common.cpp
    │      common.h
    │      IntermediateCode.cpp
    │      IntermediateCode.h
    │      
    ├─LexicalAnalyzer #词法分析 对 C 语言源代码进行词法分析，生成标记（Token）序列。
    │      lexical.cpp
    │      lexical.h
    │      
    ├─Optimize        #通过DAG优化对生成的中间语言进行优化。
    │      optimize.cpp
    │      optimize.h
    │      
    └─Parser         #编译器主题部分，读取程序文件，使用LR分析对程序先进行词法分析，随后在进行语法分析的同时进行语义分析。
            analys_LR.cpp
            analys_LR.h

```
## 构建与运行

### 先决条件

- 支持 C++11 或更高版本的编译器

### 构建步骤

1. 克隆仓库：

   ```bash
   git clone https://github.com/TruryWu/Mini-C-Compiler.git

2. 读取文法规则

   使用selectAndtable.h中的GrammarAnalyzerLR或者GrammarAnalyzerLL类对文法grammar进行分析生成ActionAndGoto表

3. 对测试程序进行编译
   
   先使用lexical.h中的Scan类对测序程序text.txt进行词法分析，然后用改Scan类初始化analys_LR.h中的Parser类，随后使用Parser类进行编译。
  
   详细使用过程可见main.cpp中的调用过程。
