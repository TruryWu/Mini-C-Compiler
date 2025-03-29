# Mini-C-Compiler

一个从零开始构建的简单C语言编译器。

## 描述

这个项目是一个用C++实现的简单的C语言编译器。该项目可以通过读取设置好的语言语法(grammer.txt)，并对代码进行词法分析，语法分析，语义分析，最终生成四元式中间代码然后对中间代码进行优化。

## 特性

- 词法分析
- 语法分析
- 语义分析
- 中间代码生成
- 优化

## 入门指南

### 先决条件

要构建和运行此项目，您需要在系统上安装以下软件：

- [GCC](https://gcc.gnu.org/) (GNU编译器集合)
- [Make](https://www.gnu.org/software/make/)

### 安装步骤

1. 克隆仓库：

    ```sh
    git clone https://github.com/TruryWu/Mini-C-Compiler.git
    cd Mini-C-Compiler
    ```

2. 使用Make构建项目：

    ```sh
    make
    ```

3. 运行编译器：

    ```sh
    ./minicc <source_file.c>
    ```

## 使用方法

要使用Mini-C-Compiler编译C源文件，请使用以下命令：

```sh
./minicc <source_file.c>
```


## 许可证

此项目使用MIT许可证。有关详细信息，请参阅LICENSE文件。

## 联系方式

- **作者：** [TruryWu](https://github.com/TruryWu)
- **仓库：** [Mini-C-Compiler](https://github.com/TruryWu/Mini-C-Compiler)

## 鸣谢

- 感谢开源社区提供的宝贵资源和工具。
