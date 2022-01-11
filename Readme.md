# 1、第一部分 Sysy -> Eeyore


## 文件结构

代码结构如下

+ build
	+ `xcw_main.l.yy.cpp`    由lex根据`xcw_scanner.l`生成的文件
	+ `xcw_parser.tab.c/xcw_parser.tab.h`  由yacc根据`xcw_parser.y`生成的文件
	+ `complier`    通过g++得到的可执行文件
+ source
	+ `xcw_scanner.l`     词法分析器 
	+ `xcw_parser.y`      语法分析器  
+ Makefile
+ tests 

## 1) 主要数据结构

### 数据类型`Ptr_num`

用来传递参数。

核心代码如下，用`IF_ptr_int`表示传上来的是否为常量，分别用`ptr_int`和`ptr_str`保存结果

```c
struct Ptr_num{
  int ptr_int;
  string ptr_str;
  int IF_ptr_int;
  Ptr_num(int p_int){
    ptr_int = p_int;
    IF_ptr_int = 1; 
  }
  Ptr_num(string p_str){
    ptr_str = p_str;
    IF_ptr_int = 0; 
  }
  Ptr_num(){}
};
```

### 符号表元素`IDENT_scope`

对于每一个IDENT（包括常量、变量、数组、函数），用一个`IDENT_scope`用于保存，其中包含了各种IDENT的基本信息。在每次词法分析器遇到IDENT时，对需要的变量进行初始化

```c
string IDENT_name;
string IDENT_num;          // 变量的值可变，因此用string存储
int  IDENT_const_num;      // const常量直接用INT型数字表示其内容
int Array_size;
vector<Ptr_num>* IDENT_array;     // 指向数组头部的指针
vector<int>* IDENT_dim_array;     // 用于存储数组的维度

int IDENT_func_param_num;      //参数的个数
int IDENT_deep;
bool IDENT_if_const;
string IR_name;          // 在Eeyore中的变量名

bool IDENT_if_array;       //是否为数组变量
bool IDENT_if_func;        //是否为函数变量
bool IDENT_if_ret_int;    //返回是否为INT
```

### 符号表`Scope`

用一个`vector`模拟符号表的结构

```c
vector<IDENT_scope> Scope;
```

函数`find_define`，输入一个IDENT，在符号表中找出这个IDENT的定义，返回指针

**实现思路就是从后往前进行寻找，遇到第一个匹配的直接返回**。

```c
IDENT_scope* find_define(string name){
    int i = Scope.size() - 1;
    if(i == -1)
        return nullptr;
    while(i >= 0){
        if(name == Scope[i].IDENT_name){
            return & Scope[i];
        }
        i--;
    }
    return nullptr;
}
```

只要保证每个Block结束后，都把这个Block中的定义的局部变量删除，这样`find_define`能找到正确的定义。具体操作是在`Block->LCURLY BlockItems RCURLY`结束后，增加如下代码

```c
int i = Scope.size() - 1;
while(Scope[i].IDENT_deep == DEEP && i >= 0){      //需要深度一致
  Scope.pop_back();
  i--;
}
DEEP --;     //还原
```

### 其他重要全局变量

+ `DEEP`：表示当前所处的深度
+ `Func_VarDecl\Func_Other` ：用于保证先输出变量定义，每次都把需要打印的语句根据是否为定义语句，分别放入这两个`vector<string>`中。

## 2） 数组定义

以比较复杂的语句`VarDef->IDENT ArrayDef ASSIGN LCURLY ArrayInit RCURLY`为例

> 即类似于 int b[4\]\[2] = {1,2,{3},{5},7,8}

### 左侧的数组定义

+ `ArrayDef`中，先把方括号中表示数组维度的参数放入全局变量`Array_dim`中，并获取当前的数组大小`n`
+ 初始化一个`IDENT_scope`变量，将`IDENT_if_array`设置为1，`Array_size`设置为n，并将`Array_dim`中的元素存到这个数组的`Ident_dim_array`中
+ 最后将数组压入到符号表中

### 右侧初始化

+ 变量`Array_dest`表示当前大括号控制的范围
+ 变量`Array_loc`表示当前读取到的位置
+ 利用数组总的size和每个维度的size，得到每个大括号的控制范围，将`Array_loc`到目标范围之间的元素都赋值成0

## 3） 数组作为值

主要是语句`LVal->IDENT ArrayLVals`

+ `Array_LVal_dim`存储了各个数组具体的维度，`Array_dim`存储了原始的维度
+ 然后根据原始数组的维度，计算每次的中间变量
+ 当数组变量不在左侧作为被赋值的对象时，需要用一个临时变量进行存储
	+ 变量`Cond_Array_Flag\R_Array_Flag\BRAC_Array_Flag`分别表示是否在条件语句、是否在右侧、是否在数组的中括号中

## 4）条件语句短路操作

+ 对于每个IF、while语句，用至少5个label表示其中的位置
	+ l0表示条件为错的位置
	+ l1表示条件为真的位置
	+ l2表示整个语句块的下一个语句
	+ l3表示当前条件语句的位置
+ 用变量`label_l_num_st`表示总的条件语句数
	+ `l_num + label_l_num_st + 2 `表示下一个条件语句的位置
+ AND语句
	+ 如果为真，直接跳到下一个条件语句
	+ 如果为假，跳到l0
+ OR语句
	+ 如果为真，跳到l1
	+ 如果为假，跳到下一个条件语句