# Mini C Compiler

## 项目简介

Mini C Compiler 是一个简单的 C 语言编译器，完全由 C++ 编写，从零开始实现了 C 语言的编译过程。项目旨在帮助学习编译原理与实现，并提供一个简洁、易于理解的编译器实现示例。
## 文法介绍
Mini C Compiler 实现的编译语言的文法包括C语言风格的变量声明，四则运算，布尔运算，for，while循环，if语句等
## 功能实现

- **词法分析**  
  对 C 语言源代码进行词法分析，生成标记（Token）序列。

- **语法分析**  
  构建抽象语法树（AST），确保源代码的语法正确性。

- **语义分析**  
  检查变量声明、作用域以及类型匹配等语义问题。

- **中间代码生成**  
  将抽象语法树转换为中间表示，以便后续处理。

- **中间代码优化**  
  对生成的中间代码进行DAG优化。

## 项目结构
```
Mini C Compile:
│  ActionAndGoto.txt
│  grammar.txt
│  tablist.txt
│  text.txt
│  textgra.txt        
└─src
    │  main.cpp
    │  
    ├─GrammerAnalyzer
    │      analys_gramer.cpp
    │      selectAndtable.h
    │      
    ├─IntermediateCode
    │      common.cpp
    │      common.h
    │      IntermediateCode.cpp
    │      IntermediateCode.h
    │      
    ├─LexicalAnalyzer
    │      lexical.cpp
    │      lexical.h
    │      
    ├─Optimize
    │      optimize.cpp
    │      optimize.h
    │      
    └─Parser
            analys_LR.cpp
            analys_LR.h

```
## 构建与运行

### 先决条件

- 支持 C++11 或更高版本的编译器
- `make` 工具

### 构建步骤

1. 克隆仓库：

   ```bash
   git clone https://github.com/TruryWu/Mini-C-Compiler.git
