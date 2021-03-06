%{
#define YYSTYPE void*
#define INTSIZE 4
#define ToStr(k) ((string*)(k))
#define ToInt(k) ((int*)(k))
#define ToPtrnum(k) ((Ptr_num*)(k))

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<vector>
#include<stack>

using namespace std;

void yyerror(const char *);
void yyerror(const string&);
extern int yylex();
extern int yyparse();
extern FILE *yyin;

ostream &out = cout;       // 用于输出

int VAR_T_num = 0, VAR_t_num = 0, VAR_p_num = 0;       //这三个全局变量分别表示Eeyore中的原生变量、临时变量和函数参数的编号

struct Ptr_num{             // 用来传递参数，用IF_ptr_int表示传上来的是否为常量
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

int DEEP;      //当前的深度
string IF_DEEP(){
    string str_if_deep = "";
    for(int i = 0;i < DEEP;i++){
        str_if_deep += "\t";
    }
    //str_if_deep = DEEP * "\t";
    return str_if_deep;
}
string IF_DEEP_DEF(){
    string str_if_deep = "";
    if(DEEP!=0)
        str_if_deep += "\t";
    //str_if_deep = DEEP * "\t";
    return str_if_deep;
}



struct IDENT_scope{     //符号表元素
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

    IDENT_scope(string name, int num, int deep, bool if_const){       //常量的构造函数
        IDENT_name = name;
        IDENT_const_num = num;
        IDENT_deep = deep;
        IDENT_if_const = if_const;
    }
    IDENT_scope(string name, string num, int deep, bool if_const){          //变量的构造函数
        IDENT_name = name;
        IDENT_num = num;
        IDENT_deep = deep;
        IDENT_if_const = if_const;
    }
};

vector<IDENT_scope> Scope;     //符号表

bool check_define(string str){       // 检查当前域中是否存在重复
    int i = Scope.size() - 1;
    if(i == -1)
        return true;
    while(Scope[i].IDENT_deep == DEEP && i >= 0){      //需要深度一致
        if(str == Scope[i].IDENT_name){
            return false;
        }
        i--;
    }
    return true;
}

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


//-----------------数组相关变量------------------------------
vector<int> Array_dim;        // 该数据结构用于数组声明时，从前到后，用于存放数组的各个维度
vector<Ptr_num> Array_LVal_dim;       //在右侧作为表达式的时候，用来存储数组对象的各个维度
stack<int> Array_LVal_length;     //在右侧作为表达式的时候，用来存储各个数组对象的长度
int path_length = 1;
int Array_loc;
int Array_dest, old_Array_dest;    //old_Array_dest用来临时存之前的Array_dest
int Array_deep;
stack<string> Array_name;

//----------------------------------------------------------

//-----------------IF_ELSE、While相关变量------------------------------
int LABEL_l_num_st = 0;       //表示一个IF语句开始，需要的label标记数   默认为0
int LABEL_l_num_end = 0;
stack<int> Stk_IF_ELSE;
int Cond_num = 0;   //Cond中的表达式数量
stack<int> Stk_END_IF_ELSE;     //记录这个循环
string str_cond;
//----------------------------------------------------------

//-----------------Continue、Break相关变量------------------------------
stack<int> Stk_Break;      // 和Stk_IF_ELSE相比，只用来储存While语句的label

//----------------------------------------------------------

//-----------------EXP相关------------------------------
string str_exp1,str_exp2;
//----------------------------------------------------------

int R_Array_Flag = 0;    //表示这个数组是否在右边表达式
string LVal_Assign_out;

int Cond_Array_Flag = 0;    //表示这个数组是否在条件表达式

stack<int> BRAC_Array_Flag;   //如果为空就说明不在括号内

int Array_in_Assign = 0;     //当且仅当数组被赋值的时候，不需要用临时变量去表示（包括定义和赋值）


//-----------------函数语句打印相关变量------------------------------
vector<string> Func_VarDecl;
vector<string> Func_Other;

//string Func_begin_flag = "funcbegin";     //这句话用来标注 函数开始
string def_out;       // 这个string 用来记录用于def的语句
string other_out;     // 这个string 用来记录其他的语句
string func_def_in = "";   // 进函数和出函数部分
string func_def_out = "";   
void Out_Print(){
    if(func_def_in != "")
        out << func_def_in << endl;
    for(int i = 0;i < Func_VarDecl.size();i++){
        out << Func_VarDecl[i] << endl;
    }
    for(int i = 0;i < Func_Other.size();i++){
        out << Func_Other[i] << endl;
    }
    //out << func_def_out << endl;
    Func_VarDecl.clear();
    Func_Other.clear();
    func_def_in = "";
}
//----------------------------------------------------------


%}

%token ADD SUB MUL DIV MOD
%token IDENT
%token LPAREN RPAREN LCURLY RCURLY LBRAC RBRAC
%token INT CONST VOID
%token LE LEQ GE GEQ EQ NEQ AND OR NOT
%token IF ELSE WHILE BREAK CONT RETURN
%token ASSIGN
%token SEMI COMMA PERIOD
%token NUMBER



%%

CompUnit:
    CompDecl
    | CompUnit CompDecl
;

CompDecl:
    DeclOutFunc
    {
        Out_Print();
    }
    | FuncDef
    {
        Out_Print();
    }
;

DeclOutFunc:
    ConstDecl
    | VarDecl
;


Decl:
    ConstDecl
    | VarDecl
;

ConstDecl:
    CONST INT ConstDefs SEMI
;

ConstDefs:
    ConstDef
    | ConstDefs COMMA ConstDef
;

ConstDef:
    IDENT ASSIGN ConstInitVal
    {
        int num = *ToInt($3);
        IDENT_scope tmp = IDENT_scope(*ToStr($1), num, DEEP, 1);
        if(check_define(*ToStr($1))){       //如果在当前域中未被定义过
            Scope.push_back(tmp);
        }
        else{
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
    | IDENT ArrayDef ASSIGN
    {
        R_Array_Flag = 1;
        // 因为常量数组的下标仍有可能是变量，因此考虑将其用变量数组的形式做
        Array_deep = 0;    //将深度初始化为0
        Array_loc = 0;    //将下标初始化为0，path_length是整个数组的长度
        old_Array_dest = 0;
        path_length = ToPtrnum($2)->ptr_int;
        //首先检查当前域中是否出现
        if(!check_define(*ToStr($1))){
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
        int n = ToPtrnum($2)->ptr_int;      //当前数组的元素总数，例如a[2][3], n=6
        def_out = IF_DEEP_DEF() + "var" + to_string(n*INTSIZE) + " T" + to_string(VAR_T_num);
        Func_VarDecl.push_back(def_out);

        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;
        IDENT_scope tmp = IDENT_scope(*ToStr($1), "0", DEEP, 1);      //这个是常量数组
        tmp.IDENT_if_array = 1;      //表示这个量是数组
        tmp.IDENT_array = Ident_array;     //指向这个新生成的vector数组
        tmp.Array_size = n;
        tmp.IR_name = "T" + to_string(VAR_T_num);
        for(int i = 0; i < Array_dim.size();i++){
            Ident_dim_array->push_back(Array_dim[i]);
        }
        tmp.IDENT_dim_array = Ident_dim_array;
        Scope.push_back(tmp);
    }
        LCURLY
        {
            Array_deep = 0;    //将深度初始化为0
            Array_loc = 0;    //将下标初始化为0，path_length是整个数组的长度
            Array_dest = Array_loc + path_length;
        }
        ArrayInit RCURLY
        {
            //没填满的元素用0填充
            for(; Array_loc < Array_dest; Array_loc++){
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                other_out = IF_DEEP() + ir_name + " = 0";
                Func_Other.push_back(other_out);
            }
            VAR_T_num ++;     //定义结束后，把变量名数字 + 1
            Array_dim.clear();     //初始化数组维度
            R_Array_Flag = 0;
        }
;

ConstInitVal: 
    ConstExp
;

ConstExp:
    AddExp
    {
        $$ = $1;
    }
;

VarDecl:
    INT VarDefs SEMI
;



VarDefs:
    VarDef
    | VarDefs COMMA VarDef
;

VarDef:
    IDENT
    {
        IDENT_scope tmp = IDENT_scope(*ToStr($1), "0", DEEP, 0);
        tmp.IR_name = "T" + to_string(VAR_T_num);
        if(check_define(*ToStr($1))){       //如果在当前域中未被定义过
            Scope.push_back(tmp);
            def_out = IF_DEEP_DEF() + "var T" + to_string(VAR_T_num);
            other_out = IF_DEEP() + "T" + to_string(VAR_T_num) + " = 0";
            Func_VarDecl.push_back(def_out);
            Func_Other.push_back(other_out);
            VAR_T_num ++ ;
        }
        else{
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
    | IDENT ASSIGN 
    {
        R_Array_Flag = 1;
    }
        InitVal
        {
            if(ToPtrnum($4)->IF_ptr_int == 1){       //传递的是常量
                int num = ToPtrnum($4)->ptr_int;
                IDENT_scope tmp = IDENT_scope(*ToStr($1), to_string(num), DEEP, 0);  
                tmp.IR_name = "T" + to_string(VAR_T_num);   
                Scope.push_back(tmp);
                def_out = IF_DEEP_DEF() + "var T" + to_string(VAR_T_num);
                other_out = IF_DEEP() + "T" + to_string(VAR_T_num) + " = " + to_string(num);
                Func_VarDecl.push_back(def_out);
                Func_Other.push_back(other_out);
                VAR_T_num ++ ;
            }
            else{              //传递的是变量
                string num = ToPtrnum($4)->ptr_str;

                IDENT_scope tmp = IDENT_scope(*ToStr($1), num, DEEP, 0);
                tmp.IR_name = "T" + to_string(VAR_T_num);   
                Scope.push_back(tmp);
                def_out = IF_DEEP_DEF() + "var T" + to_string(VAR_T_num);

                other_out = IF_DEEP() + "T" + to_string(VAR_T_num) + " = " + num;
                Func_VarDecl.push_back(def_out);
                Func_Other.push_back(other_out);
                VAR_T_num ++ ;
            }
            R_Array_Flag = 0;
        }
    | IDENT ArrayDef
    {
        //首先检查当前域中是否出现
        if(!check_define(*ToStr($1))){
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }

        int n = ToPtrnum($2)->ptr_int;      //当前数组的元素总数，例如a[2][3], n=6
        def_out = IF_DEEP_DEF() + "var " + to_string(n*INTSIZE) + " T" + to_string(VAR_T_num);
        Func_VarDecl.push_back(def_out);
        
        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;
        IDENT_scope tmp = IDENT_scope(*ToStr($1), "0", DEEP, 0);
        tmp.IDENT_if_array = 1;      //表示这个量是数组
        tmp.IDENT_array = Ident_array;     //指向这个新生成的vector数组
        tmp.Array_size = n;
        tmp.IR_name = "T" + to_string(VAR_T_num);
        for(int i = 0; i < Array_dim.size();i++){
            Ident_dim_array->push_back(Array_dim[i]);
        }
        tmp.IDENT_dim_array = Ident_dim_array;
        Scope.push_back(tmp);

        for(int i = 0; i < n; i++){
            Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * i) + "]";
            Ident_array->push_back(tmp_ptr);
        }
        VAR_T_num ++;

        Array_dim.clear();
    }
    | IDENT ArrayDef ASSIGN 
    {
        R_Array_Flag = 1;
        
        // 先进行初始化
        Array_deep = 0;    //将深度初始化为0
        Array_loc = 0;    //将下标初始化为0，path_length是整个数组的长度
        old_Array_dest = 0;
        path_length = ToPtrnum($2)->ptr_int;
        // 对应 a[4][2] = {1,2,{3},{5},7,8} 这些情况

        //首先检查当前域中是否出现
        if(!check_define(*ToStr($1))){
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
        int n = ToPtrnum($2)->ptr_int;      //当前数组的元素总数，例如a[2][3], n=6
        def_out = IF_DEEP_DEF() + "var " + to_string(n*INTSIZE) + " T" + to_string(VAR_T_num);
        Func_VarDecl.push_back(def_out);

        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;
        IDENT_scope tmp = IDENT_scope(*ToStr($1), "0", DEEP, 0);
        tmp.IDENT_if_array = 1;      //表示这个量是数组
        tmp.IDENT_array = Ident_array;     //指向这个新生成的vector数组
        tmp.Array_size = n;
        tmp.IR_name = "T" + to_string(VAR_T_num);
        // 把Array_dim中的内容复制到IDENT_dim_array中
        for(int i = 0; i < Array_dim.size();i++){
            Ident_dim_array->push_back(Array_dim[i]);
        }
        tmp.IDENT_dim_array = Ident_dim_array;
        Scope.push_back(tmp);
    }
        LCURLY
        {
            Array_dest = Array_loc + path_length;
        }
        ArrayInit RCURLY
        {
            //没填满的元素用0填充
            for(; Array_loc < Array_dest; Array_loc++){
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                //out << IF_DEEP() + ir_name << " = " << 0 << endl;
                other_out = IF_DEEP() + ir_name + " = 0";
                Func_Other.push_back(other_out);
            }
            VAR_T_num ++;     //定义结束后，把变量名数字 + 1
            Array_dim.clear();     //初始化数组维度

            R_Array_Flag = 0;
        }
;

ArrayInit:
    {
        // 类似于{}的情况，推出空值
    }
    | ArrayExps
    {
        //类似于 {1},{1,2}
        $$ = $1;
    }
;

ArrayExps:
    ArrayExp
    | ArrayExps COMMA ArrayExp
;

ArrayExp:
    Exp
    {
        if(ToPtrnum($1)->IF_ptr_int){    //为常量，加入到数组中
            Ptr_num tmp_ptr = Ptr_num(ToPtrnum($1)->ptr_int);     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
            Scope.back().IDENT_array->push_back(tmp_ptr);
            other_out = IF_DEEP() + ir_name + " = " + to_string(ToPtrnum($1)->ptr_int);
            Func_Other.push_back(other_out);
        }       
        else{          //为变量，加入到数组中
            Ptr_num tmp_ptr = Ptr_num(ToPtrnum($1)->ptr_str);     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
            Scope.back().IDENT_array->push_back(tmp_ptr);
            other_out = IF_DEEP() + ir_name + " = " + ToPtrnum($1)->ptr_str;
            Func_Other.push_back(other_out);
        }
        Array_loc ++;     //位置向前进1
    }
    | LCURLY 
    {
        path_length = path_length / Array_dim[Array_deep];
        Array_deep ++;      //遇到左括号，深度+1
        old_Array_dest = Array_dest;
        Array_dest = Array_loc + path_length;
    }
        ArrayInit RCURLY
        {
            for(; Array_loc < Array_dest; Array_loc++){
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                other_out = IF_DEEP() + ir_name + " = 0";
                Func_Other.push_back(other_out);   
            }
            Array_dest = old_Array_dest;
            Array_deep --;      //遇到右括号，深度-1
            path_length = path_length * Array_dim[Array_deep];
        }

;



ArrayDef:
    ArrayUnit
    {
        $$ = $1;
    }
    | ArrayDef ArrayUnit
    {
        ToPtrnum($1)->ptr_int = ToPtrnum($1)->ptr_int * ToPtrnum($2)->ptr_int;
        $$ = $1;
    }
;

ArrayUnit:
    LBRAC ConstExp RBRAC
    {
        $$ = $2;
        Array_dim.push_back(ToPtrnum($2)->ptr_int);      //把数组数据放到Array_dim中，记录数组维度信息
        
    }
;

InitVal: 
    Exp
    {
        $$ = $1;
    }
;

Exp:
    AddExp
    {
        $$ = $1;            
    }
;

AddExp:
    MulExp
    {
        $$ =$1;
    }
    | AddExp ADD MulExp
    {
        Ptr_num* add_1 = ToPtrnum($1);
        Ptr_num* mul_1 = ToPtrnum($3);
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            tmp_ptr->ptr_int = add_1->ptr_int + mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            if(add_1->IF_ptr_int){
                str_exp1 = to_string(add_1->ptr_int);
            }
            else{
                str_exp1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                str_exp2 = to_string(mul_1->ptr_int);
            }
            else{
                str_exp2 = mul_1->ptr_str;
            }

            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            other_out = IF_DEEP() + tmp_ptr->ptr_str + " = " + str_exp1 + " + " + str_exp2;
            Func_Other.push_back(other_out);
        }
        $$ = tmp_ptr; 
    }
    | AddExp SUB MulExp
    {
        Ptr_num* add_1 = ToPtrnum($1);
        Ptr_num* mul_1 = ToPtrnum($3);

        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            tmp_ptr->ptr_int = add_1->ptr_int - mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            if(add_1->IF_ptr_int){
                str_exp1 = to_string(add_1->ptr_int);
            }
            else{
                str_exp1 = add_1->ptr_str;
                //out <<  "str_exp1 = "<<str_exp1 << endl;
            }
            if(mul_1->IF_ptr_int){
                str_exp2 = to_string(mul_1->ptr_int);
            }
            else{
                str_exp2 = mul_1->ptr_str;
            }
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            other_out = IF_DEEP() + tmp_ptr->ptr_str + " = " + str_exp1 + " - " + str_exp2;
            Func_Other.push_back(other_out);
        }
        $$ = tmp_ptr; 
    }

;

MulExp:
    UnaryExp
    {
        $$ = $1;
    }
    | MulExp MUL UnaryExp
    {
        Ptr_num* add_1 = ToPtrnum($1);
        Ptr_num* mul_1 = ToPtrnum($3);
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            tmp_ptr->ptr_int = add_1->ptr_int * mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            if(add_1->IF_ptr_int){
                str_exp1 = to_string(add_1->ptr_int);
            }
            else{
                str_exp1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                str_exp2 = to_string(mul_1->ptr_int);
            }
            else{
                str_exp2 = mul_1->ptr_str;
            }
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;

            other_out = IF_DEEP() + tmp_ptr->ptr_str + " = " + str_exp1 + " * " + str_exp2;
            Func_Other.push_back(other_out);
        }
        $$ = tmp_ptr; 
    }
    | MulExp DIV UnaryExp
    {
        Ptr_num* add_1 = ToPtrnum($1);
        Ptr_num* mul_1 = ToPtrnum($3);
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            if(mul_1->ptr_int == 0){       //除0报错
                yyerror("Integer division by zero.");
            }
            tmp_ptr->ptr_int = add_1->ptr_int / mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            if(add_1->IF_ptr_int){
                str_exp1 = to_string(add_1->ptr_int);
            }
            else{
                str_exp1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                if(mul_1->ptr_int == 0){       //除0报错
                    yyerror("Integer division by zero.");
                }
                str_exp2 = to_string(mul_1->ptr_int);
            }
            else{
                str_exp2 = mul_1->ptr_str;
            }
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            other_out = IF_DEEP() + tmp_ptr->ptr_str + " = " + str_exp1 + " / " + str_exp2;
            Func_Other.push_back(other_out);
        }
        $$ = tmp_ptr; 
    }
    | MulExp MOD UnaryExp
    {
        Ptr_num* add_1 = ToPtrnum($1);
        Ptr_num* mul_1 = ToPtrnum($3);
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            if(mul_1->ptr_int == 0){       //除0报错
                yyerror("Integer mod by zero.");
            }
            tmp_ptr->ptr_int = add_1->ptr_int % mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            if(add_1->IF_ptr_int){
                str_exp1 = to_string(add_1->ptr_int);
            }
            else{
                str_exp1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                if(mul_1->ptr_int == 0){       //除0报错
                    yyerror("Integer mod by zero.");
                }
                str_exp2 = to_string(mul_1->ptr_int);
            }
            else{
                str_exp2 = mul_1->ptr_str;
            }
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            other_out = IF_DEEP() + tmp_ptr->ptr_str + " = " + str_exp1 + " % " + str_exp2;
            Func_Other.push_back(other_out);
        }
        $$ = tmp_ptr; 
    }
;

UnaryExp:
    PrimaryExp
    {
        $$ = $1;
    }
    | IDENT LPAREN 
    {
        BRAC_Array_Flag.push(1);
    }
    FuncRParams RPAREN
    {
        IDENT_scope* tmp = find_define(*ToStr($1));

        if( tmp == nullptr){          //变量尚未定义
            string err = "\"" +  *ToStr($1) + "\" was not declared in this scope.";
            yyerror(err);
        }

        if(tmp->IDENT_if_ret_int == 1){        //返回值是INT的函数
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            other_out = IF_DEEP() + "t" + to_string(VAR_t_num) + " = call f_" + *(ToStr($1));
            Func_Other.push_back(other_out);

            Ptr_num* tmp_ptr = new Ptr_num;
            tmp_ptr->ptr_str = "t" + to_string(VAR_t_num);
            tmp_ptr->IF_ptr_int = 0;

            VAR_t_num ++;
            $$ = tmp_ptr;
        }
        else{
            other_out = IF_DEEP() + "call f_" + *(ToStr($1));
            Func_Other.push_back(other_out);
        }
        BRAC_Array_Flag.pop();
    }
    | ADD UnaryExp
    {
        $$ = $2;
    }
    | SUB UnaryExp
    {
        Ptr_num* tmp_ptr = ToPtrnum($2);
        if(tmp_ptr->IF_ptr_int){
            tmp_ptr->ptr_int = - tmp_ptr->ptr_int;
        }
        else{
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            other_out = IF_DEEP() + "t" + to_string(VAR_t_num ) + " = -" + tmp_ptr->ptr_str;
            Func_Other.push_back(other_out);
            tmp_ptr->ptr_str = "t" + to_string(VAR_t_num );
            VAR_t_num ++;
        }
        $$ = $2;
    }
    | NOT UnaryExp
    {
        Ptr_num* tmp_ptr = ToPtrnum($2);
        if(tmp_ptr->IF_ptr_int){
            if(tmp_ptr->ptr_int == 0)
                tmp_ptr->ptr_int = 1;
            else    
                tmp_ptr->ptr_int = 0;
        }
        else{
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            other_out = IF_DEEP() + "t" + to_string(VAR_t_num ) + " = !" + tmp_ptr->ptr_str;
            Func_Other.push_back(other_out);
            tmp_ptr->ptr_str = "t" + to_string(VAR_t_num );
            VAR_t_num ++;
        }
        $$ = $2;
    }
;



PrimaryExp:
    NUMBER
    {
        Ptr_num* tmp_ptr = new Ptr_num;
        tmp_ptr->ptr_int = *ToInt($1);
        tmp_ptr->IF_ptr_int = 1;
        $$ = tmp_ptr;
    }
    | LVal
    {
        $$ = $1;
        
    }
    | LPAREN Exp RPAREN
    {
        $$ = $2;
    }
;

LVal:
    IDENT
    {
        IDENT_scope* tmp = find_define(*ToStr($1));
        if( tmp == nullptr){          //变量尚未定义
            string err = "\"" +  *ToStr($1) + "\" was not declared in this scope.";
            yyerror(err);
        }
        Ptr_num* tmp_ptr = new Ptr_num;
        if(tmp->IDENT_if_const){      //是常量，此时直接返回数值
            tmp_ptr->ptr_int = tmp->IDENT_const_num;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{       //是变量，此时返回类似于T0
            tmp_ptr->ptr_str = tmp->IR_name;
            tmp_ptr->IF_ptr_int = 0;
        }   
        $$ = tmp_ptr;
    }
    | IDENT
    {
        IDENT_scope* tmp = find_define(*ToStr($1));    //搜索这个数组的定义
        Array_name.push(tmp->IR_name);

        Array_LVal_length.push(0);
    }
        ArrayLVals
        {        //a[2][3]     a[1][b]  

            IDENT_scope* tmp_lval = find_define(*ToStr($1));    //搜索这个数组的定义
            Array_dim.clear();
            for(int i = 0;i < (*tmp_lval).IDENT_dim_array->size();i++){
                Array_dim.push_back((*(*tmp_lval).IDENT_dim_array)[i]);    //原数组的维度
            }
            Ptr_num* tmp_ptr = new Ptr_num;      //尝试写成指针
            Ptr_num* tmp_ptr_new = new Ptr_num;
            Ptr_num* tmp_ptr_old = new Ptr_num;
            int ptr_size = INTSIZE;
            
            //  Array_LVal_dim 存储了各个数组具体的维度，Array_dim存储了原始的维度
            int array_len = Array_LVal_length.top();
            for(int i = 0; i < array_len ;i ++){
                tmp_ptr = & Array_LVal_dim.back();
                Array_LVal_dim.pop_back();
                if(tmp_ptr->IF_ptr_int){     //是整数
                    tmp_ptr_new->IF_ptr_int = 1;
                    tmp_ptr_new->ptr_int = tmp_ptr->ptr_int * ptr_size;
                    if(i != 0){     //第一次，不用考虑和之前相加
                        if(tmp_ptr_old->IF_ptr_int){     //如果前面的也是INT
                            tmp_ptr_old->ptr_int += tmp_ptr_new->ptr_int;
                        }
                        else{
                            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
                            Func_VarDecl.push_back(def_out);
                            other_out =  IF_DEEP() + "t" + to_string(VAR_t_num ) + " = " + to_string(tmp_ptr_new->ptr_int) + " + " + tmp_ptr_old->ptr_str;
                            Func_Other.push_back(other_out);
                            tmp_ptr_old->ptr_str = "t" + to_string(VAR_t_num);
                            VAR_t_num ++;
                        }
                    }
                    else{
                        tmp_ptr_old->ptr_int = tmp_ptr_new->ptr_int;
                        tmp_ptr_old->IF_ptr_int = tmp_ptr_new->IF_ptr_int;
                    }
                }
                else{            //其中有一个是变量
                    def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
                    Func_VarDecl.push_back(def_out);
                    tmp_ptr_new->IF_ptr_int = 0;
                    tmp_ptr_new->ptr_str = "t" + to_string(VAR_t_num);
                    
                    if(i != 0 && tmp_ptr_old->IF_ptr_int){
                        tmp_ptr_old->ptr_str = to_string(tmp_ptr_old->ptr_int);    //强制转换为string类型
                        tmp_ptr_old->IF_ptr_int = 0;    
                    }
                    VAR_t_num ++;

                    other_out = IF_DEEP() + tmp_ptr_new->ptr_str + " = " + tmp_ptr->ptr_str + " * " + to_string(ptr_size);
                    Func_Other.push_back(other_out);
                    if(i != 0){     //第一次不用考虑和之前相加
                        
                        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
                        Func_VarDecl.push_back(def_out);

                        other_out = IF_DEEP() + "t" + to_string(VAR_t_num) + " = " + tmp_ptr_new->ptr_str + " + " + tmp_ptr_old->ptr_str;
                        Func_Other.push_back(other_out);
                        tmp_ptr_old->ptr_str = "t" + to_string(VAR_t_num);
                        VAR_t_num ++;
                    }
                    else{     //注意这里需要深度复制
                        tmp_ptr_old->ptr_str = tmp_ptr_new->ptr_str;
                        tmp_ptr_old->IF_ptr_int = tmp_ptr_new->IF_ptr_int;
                    }
                }
                ptr_size *= Array_dim.back();     //更新ptr_size
                Array_dim.pop_back();
            }
            if(tmp_ptr_old->IF_ptr_int){
                tmp_ptr_old->ptr_str = Array_name.top() + "[" + to_string(tmp_ptr_old->ptr_int) + "]";
                tmp_ptr_old->IF_ptr_int = 0;     //最后的结果一定是一个字符串类型
            }
            else{
                tmp_ptr_old->ptr_str = Array_name.top() + "[" + tmp_ptr_old->ptr_str + "]";
                tmp_ptr_old->IF_ptr_int = 0;
            }    

            if(Cond_Array_Flag == 1 || R_Array_Flag == 1 || (!BRAC_Array_Flag.empty())){      //表示位于右侧的数组

                def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
                Func_VarDecl.push_back(def_out);
                other_out = IF_DEEP() + "t" + to_string(VAR_t_num ) + " = " + tmp_ptr_old->ptr_str;
                Func_Other.push_back(other_out);

                tmp_ptr_old->ptr_str = "t" + to_string(VAR_t_num);
                VAR_t_num ++;
                $$ = tmp_ptr_old;
            }
            else{
                $$ = tmp_ptr_old;
            }
            Array_name.pop();
            Array_LVal_length.pop();
        }
;

ArrayLVals:
    ArrayLVal
    | ArrayLVals ArrayLVal
;

ArrayLVal:
    LBRAC 
    {
        BRAC_Array_Flag.push(1);
    }
    Exp RBRAC
    {
        Array_LVal_dim.push_back(*(ToPtrnum($3)));    //存入进行引用的维度
        BRAC_Array_Flag.pop();
        Array_LVal_length.top() += 1;      //长度 + 1
    }
;


FuncDef:
    INT IDENT
    {
        //首先检查当前域中是否出现
        if(!check_define(*ToStr($2))){
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
        //  开始函数定义
        IDENT_scope tmp = IDENT_scope(*ToStr($2), "0", DEEP, 0);    //不是Const
        tmp.IDENT_if_ret_int = 1;
        Scope.push_back(tmp);
        
    }
        LPAREN 
        {
            DEEP ++;
        }
        FuncFParams RPAREN
        {
            DEEP --;
            func_def_in =  "f_" + *ToStr($2) + " [" + to_string(VAR_p_num) + "]";
        }
        Block
        {
            other_out = "\treturn 0";
            Func_Other.push_back(other_out);
            other_out = "end f_" + *ToStr($2);
            Func_Other.push_back(other_out);
            //声明结束后，把记录参数数量的 VAR_p_num 初始化
            VAR_p_num = 0;
            
        }
    | VOID IDENT
    {
        //首先检查当前域中是否出现
        if(!check_define(*ToStr($2))){
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
        //  开始函数定义
        IDENT_scope tmp = IDENT_scope(*ToStr($2), "0", DEEP, 0);    //是Const,表示无返回值
        tmp.IDENT_if_ret_int = 0;
        Scope.push_back(tmp);
    }
        LPAREN
        {
            DEEP ++;
        }
        FuncFParams RPAREN
        {
            DEEP --;
            func_def_in = "f_" + (*ToStr($2)) + " [" + to_string(VAR_p_num) + "]";
        }
        Block
        {
            other_out = "\treturn";
            Func_Other.push_back(other_out);
            other_out = "end f_" + *ToStr($2);
            Func_Other.push_back(other_out);

            //声明结束后，把记录参数数量的 VAR_p_num 初始化
            VAR_p_num = 0;
        }
;


FuncFParams:
    {
        //表示没有参数的情况
    }
    | FuncFParam
    {

    }
    | FuncFParams COMMA FuncFParam
;

FuncFParam:    
    INT IDENT
    { 
        // 检查是否出现过
        if(!check_define(*ToStr($2))){
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
        
        IDENT_scope tmp = IDENT_scope(*ToStr($2), "0", DEEP, 0);    //是一个变量
        tmp.IDENT_if_array = 0;    //不是数组
        tmp.IR_name = "p" + to_string(VAR_p_num);
        //tmp.Print_IDENT();
        VAR_p_num ++;
        Scope.push_back(tmp);
    }
    | INT IDENT LBRAC 
    {
        Array_dim.push_back(10);   //传入一个假参数
    }
    RBRAC ArrayParamDef
    {      //void d(int d[])
        if(!check_define(*ToStr($2))){
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }

        IDENT_scope* tmp_ptr = new IDENT_scope(*ToStr($2), "0", DEEP, 0);    //是一个变量
        tmp_ptr->IDENT_if_array = 1;    //是数组
        tmp_ptr->IR_name = "p" + to_string(VAR_p_num);

        int n = ToPtrnum($6)->ptr_int;

        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;

        tmp_ptr->Array_size = n;
        for(int i = 0; i < Array_dim.size();i++){
            Ident_dim_array->push_back(Array_dim[i]);
        }
        tmp_ptr->IDENT_dim_array = Ident_dim_array;

        VAR_p_num ++;
        Scope.push_back(*tmp_ptr);

        Array_dim.clear();
    }
;

ArrayParamDef:
    ArrayUnit
    {
        $$ = $1;
    }
    | ArrayParamDef ArrayUnit
    {   //   例如[3][4]
        ToPtrnum($1)->ptr_int = ToPtrnum($1)->ptr_int * ToPtrnum($2)->ptr_int;
        $$ = $1;
    }
    |
    {
        Ptr_num* tmp_dim = new Ptr_num(1);     //传一个假的n上去 
        $$ = (void*)tmp_dim;
    }
;

Block:
    LCURLY
    {
        DEEP ++;     // 深度+1，保证这些新加入的元素属于当前域中
    }
        BlockItems RCURLY
        {
            //结束以后，删除在其中定义过的变量,因为这些实际上是局部变量，但方便起见输出成原生变量
            //在这里正好也同时删除了参数变量（因为事实上声明时并不会用到）
            int i = Scope.size() - 1;
            while(Scope[i].IDENT_deep == DEEP && i >= 0){      //需要深度一致
                Scope.pop_back();
                i--;
            }
            DEEP --;     //还原
        }
;

BlockItems:
    BlockItem
    | BlockItems BlockItem;

BlockItem:
    {
        //暂时先考虑内容为空的情况
    }
    | Decl
    {
    }
    | Stmt
    {
    }
;

Stmt:
    RETURN SEMI
    {
    }
    | RETURN
    {
        BRAC_Array_Flag.push(1);
    }
    Exp SEMI
    {
        if(ToPtrnum($3)->IF_ptr_int){       //为常量
            other_out = IF_DEEP() + "return " + to_string(ToPtrnum($3)->ptr_int);
            Func_Other.push_back(other_out);
        }
        else{
            other_out = IF_DEEP() + "return " + ToPtrnum($3)->ptr_str;
            Func_Other.push_back(other_out);
        }
        BRAC_Array_Flag.pop();
    }
    | Exp SEMI
    {
        //类似于 直接调用void函数，如f(1,2);
    }
    | SEMI
    {
        // 一个;的情况
    }
    | Block
    {
        // 内部还是一个语句块
    }
    | LVal 
    {
        // out << IF_DEEP() + ToPtrnum($1)->ptr_str;
        LVal_Assign_out = IF_DEEP() + ToPtrnum($1)->ptr_str;
        R_Array_Flag = 1;
    }
        ASSIGN Exp SEMI
        {
            //类似于 a = b   LVal 返回一个 Ptr_num类型的指针tmp_ptr，此时LVal传上来的必定是一个ptr_str
            
            if(ToPtrnum($4)->IF_ptr_int == 1){       //传递的是常量
                int num = ToPtrnum($4)->ptr_int;
                // out << " = " << num << endl;
                LVal_Assign_out += (" = " + to_string(num));
                Func_Other.push_back(LVal_Assign_out);
            }
            else{              //Exp传递的是变量
                string num = ToPtrnum($4)->ptr_str;
                LVal_Assign_out += (" = " + num);
                Func_Other.push_back(LVal_Assign_out);
            }
            R_Array_Flag = 0;

        }
    | IF 
    {
        LABEL_l_num_st = LABEL_l_num_end;
        Stk_IF_ELSE.push(LABEL_l_num_st);      //当前的label存入栈中
    }
        LPAREN Cond RPAREN 
        {
            if(ToPtrnum($4)->IF_ptr_int){    //如果是常量
                str_cond = to_string(ToPtrnum($4)->ptr_int);
            }
            else{
                str_cond = ToPtrnum($4)->ptr_str;
            }
            LABEL_l_num_st = Stk_IF_ELSE.top();

            other_out = IF_DEEP() + "\tif " + str_cond + " == 0 goto l" + to_string(LABEL_l_num_st);
            Func_Other.push_back(other_out);
            other_out = IF_DEEP() + "\tgoto l" + to_string(LABEL_l_num_st+1);   //正确跳到l1
            Func_Other.push_back(other_out);

            other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+1) + ":";    // l1后面是正确内容
            Func_Other.push_back(other_out);

            LABEL_l_num_end = Stk_IF_ELSE.top() + 5 + Cond_num;    // 正常会用4个
            Cond_num = 0;    // 初始化
        }
        Stmt
        { 
            LABEL_l_num_st = Stk_IF_ELSE.top();
            
            other_out = IF_DEEP() + "\t" + "goto l" + to_string(LABEL_l_num_st+2);   
            Func_Other.push_back(other_out);

            other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st) + ":";      // 写l0
            Func_Other.push_back(other_out);    

            
        }
        IF_Else
        {
            
            Stk_IF_ELSE.pop();
        }
    | WHILE 
    {
        LABEL_l_num_st = LABEL_l_num_end;
        Stk_IF_ELSE.push(LABEL_l_num_st);      //当前的label存入栈中
        Stk_Break.push(LABEL_l_num_st);
    }
        LPAREN Cond RPAREN
        {
            // 处理最后一条指令
            if(ToPtrnum($4)->IF_ptr_int){    //如果是常量
                str_cond = to_string(ToPtrnum($4)->ptr_int);
            }
            else{
                str_cond = ToPtrnum($4)->ptr_str;
            }
            LABEL_l_num_st = Stk_IF_ELSE.top();
            other_out = IF_DEEP() + "\tif " + str_cond + " == 0 goto l" + to_string(LABEL_l_num_st);
            Func_Other.push_back(other_out);
            other_out = IF_DEEP() + "\tgoto l" + to_string(LABEL_l_num_st+1);   //正确跳到下一行
            Func_Other.push_back(other_out);

            other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+1) + ":";
            Func_Other.push_back(other_out);  

            LABEL_l_num_end = LABEL_l_num_st + 5 + Cond_num;
            Cond_num = 0;
        }
        Stmt
        {
            LABEL_l_num_st = Stk_IF_ELSE.top();
            Stk_IF_ELSE.pop();
            Stk_Break.pop();

            other_out = IF_DEEP() + "\t" + "goto l" + to_string(LABEL_l_num_st+3);    //跳到开始的地方
            Func_Other.push_back(other_out);  

            other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st) + ":";     //错误的情况
            Func_Other.push_back(other_out);              
            other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":";    //下一个句子
            Func_Other.push_back(other_out);  

            
        }
    | BREAK SEMI
    {
        LABEL_l_num_st = Stk_Break.top();
        other_out = IF_DEEP() + "goto l" + to_string(LABEL_l_num_st+2);    //l2表示下一个句子
        Func_Other.push_back(other_out);  
    }
    | CONT SEMI
    {
        LABEL_l_num_st = Stk_Break.top();
        other_out = IF_DEEP() + "goto l" + to_string(LABEL_l_num_st+3);     //l3表示自己本身
        Func_Other.push_back(other_out);  
    }
;



IF_Else:
    {
        // 不存在ELSE的情况
        LABEL_l_num_st = Stk_IF_ELSE.top();

        // 跳到结束的位置
        other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":";
        Func_Other.push_back(other_out);  
    }
    | ELSE Stmt
    {
        LABEL_l_num_st = Stk_IF_ELSE.top();
        //Stk_IF_ELSE.pop();
        // out << IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":" << endl;
        other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":";
        Func_Other.push_back(other_out);  
    }
;

Cond:
    {
        Cond_Array_Flag = 1;
        other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+3) + ":";      // 写l3
        Func_Other.push_back(other_out);
    }
    LOrExp
    {
        $$ = $2;
        Cond_Array_Flag = 0;
    }
;

LOrExp:
    LAndExp
    {
         $$ = $1;
    }
    | LOrExp OR 
    {
        Cond_num += 1;

        if(ToPtrnum($1)->IF_ptr_int){    //如果是常量
            str_cond = to_string(ToPtrnum($1)->ptr_int);
        }
        else{
            str_cond = ToPtrnum($1)->ptr_str;
        }  
        LABEL_l_num_st = Stk_IF_ELSE.top();

        other_out = IF_DEEP() + "\tif " + str_cond + " == 0 goto l" + to_string(LABEL_l_num_st+3+Cond_num);
        Func_Other.push_back(other_out);

        other_out = IF_DEEP() + "\tgoto l" + to_string(LABEL_l_num_st+1);
        Func_Other.push_back(other_out);

        other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+3+Cond_num) + ":";      // 写l3
        Func_Other.push_back(other_out);  
    }
    LAndExp
    {
        $$ = $4;
    }
;

LAndExp:
    EqExp
    {
        $$ = $1;
    }
    | LAndExp AND 
    {
        Cond_num += 1;
        

        if(ToPtrnum($1)->IF_ptr_int){    //如果是常量
            str_cond = to_string(ToPtrnum($1)->ptr_int);
        }
        else{
            str_cond = ToPtrnum($1)->ptr_str;
        }
        LABEL_l_num_st = Stk_IF_ELSE.top();
        other_out = IF_DEEP() + "\tif " + str_cond + " == 0 goto l" + to_string(LABEL_l_num_st);
        Func_Other.push_back(other_out);
        other_out = IF_DEEP() + "\tgoto l" + to_string(LABEL_l_num_st+3+Cond_num);   //正确跳到下一行
        Func_Other.push_back(other_out);

        other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+3+Cond_num) + ":";      // 写l3
        Func_Other.push_back(other_out);  
    }
    EqExp
    {
        $$ = $4;  
    }
;

EqExp:
    RelExp
    {
        $$ = $1;
    }
    | EqExp EQ RelExp
    {
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum($1)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($1)->ptr_int);
        }
        else{
            other_out += ToPtrnum($1)->ptr_str;
        }
        other_out += " == ";
        if(ToPtrnum($3)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($3)->ptr_int);
        }
        else{
            other_out += ToPtrnum($3)->ptr_str;
        }
        Func_Other.push_back(other_out); 
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out); 
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        $$ = tmp_ptr;
    }
    | EqExp NEQ RelExp
    {
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum($1)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($1)->ptr_int);
        }
        else{
            other_out += ToPtrnum($1)->ptr_str;
        }
        other_out += " != ";
        if(ToPtrnum($3)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($3)->ptr_int);
        }
        else{
            other_out += ToPtrnum($3)->ptr_str;
        }
        Func_Other.push_back(other_out);  
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out);
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        $$ = tmp_ptr;
    }
;

RelExp:
    AddExp
    {
        $$ = $1;
    }
    | RelExp LE AddExp
    {
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum($1)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($1)->ptr_int);
        }
        else{
            other_out += ToPtrnum($1)->ptr_str;
        }
        other_out += " < ";
        if(ToPtrnum($3)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($3)->ptr_int);
        }
        else{
            other_out += ToPtrnum($3)->ptr_str;
        }
        Func_Other.push_back(other_out);  
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out);
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        $$ = tmp_ptr;
    }
    | RelExp GE AddExp
    {
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum($1)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($1)->ptr_int);
        }
        else{
            other_out += ToPtrnum($1)->ptr_str;
        }
        other_out += " > ";
        if(ToPtrnum($3)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($3)->ptr_int);
        }
        else{
            other_out += ToPtrnum($3)->ptr_str;
        }
        Func_Other.push_back(other_out);  
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out);
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        $$ = tmp_ptr;
    }
    | RelExp LEQ AddExp
    {
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum($1)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($1)->ptr_int);
        }
        else{
            other_out += ToPtrnum($1)->ptr_str;
        }
        other_out += " <= ";
        if(ToPtrnum($3)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($3)->ptr_int);
        }
        else{
            other_out += ToPtrnum($3)->ptr_str;
        }
        Func_Other.push_back(other_out);  
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out);
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        $$ = tmp_ptr;
    }
    | RelExp GEQ AddExp
    {
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum($1)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($1)->ptr_int);
        }
        else{
            other_out += ToPtrnum($1)->ptr_str;
        }
        other_out += " >= ";
        if(ToPtrnum($3)->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum($3)->ptr_int);
        }
        else{
            other_out += ToPtrnum($3)->ptr_str;
        }
        Func_Other.push_back(other_out);  
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out);
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        $$ = tmp_ptr;
    }
;



FuncRParams:
    {
        //表示函数调用中没有参数的情况
    }
    | Exp
    {
        Ptr_num* param_tmp = ToPtrnum($1);
        if(param_tmp->IF_ptr_int){       //如果是常量
            // out << IF_DEEP() + "param " << param_tmp->ptr_int << endl;
            other_out = IF_DEEP() + "param " + to_string(param_tmp->ptr_int);
            Func_Other.push_back(other_out);  
        }
        else{
            // out << IF_DEEP() + "param " + param_tmp->ptr_str << endl;
            other_out = IF_DEEP() + "param " + param_tmp->ptr_str ;
            Func_Other.push_back(other_out);  
        }

    }
    | FuncRParams COMMA Exp
    {
        Ptr_num* param_tmp = ToPtrnum($3);
        if(param_tmp->IF_ptr_int){       //如果是常量
            // out << IF_DEEP() + "param " << param_tmp->ptr_int << endl;
            other_out = IF_DEEP() + "param " + to_string(param_tmp->ptr_int);
            Func_Other.push_back(other_out);
        }
        else{
            // out << IF_DEEP() + "param " + param_tmp->ptr_str << endl;
            other_out = IF_DEEP() + "param " + param_tmp->ptr_str ;
            Func_Other.push_back(other_out); 
        }
    }
;










%%

void yyerror(const char *s) {
    extern int yylineno;
    cout << "Line " << yylineno <<  ": " << s << endl;
    exit(1);
}

void yyerror(const string &s) {
    yyerror(s.c_str());
}

int main(int argc, char **argv) {
    if (argc >= 4)
        if ((yyin = fopen(argv[3], "r")) == NULL)
            yyerror("Cannot open input file.");
    
    if (argc >= 6)
        if (freopen(argv[5], "w", stdout) == NULL)
            yyerror("Cannot open output file.");

    IDENT_scope tmp_1 = IDENT_scope("getint", "0", DEEP, 0);    //不是Const
    tmp_1.IDENT_if_ret_int = 1;
    Scope.push_back(tmp_1);
    IDENT_scope tmp_2 = IDENT_scope("getch", "0", DEEP, 0);    //不是Const
    tmp_2.IDENT_if_ret_int = 1;
    Scope.push_back(tmp_2);
    IDENT_scope tmp_3 = IDENT_scope("getarray", "0", DEEP, 0);    //不是Const
    tmp_3.IDENT_if_ret_int = 1;
    Scope.push_back(tmp_3);
    IDENT_scope tmp_4 = IDENT_scope("putint", "0", DEEP, 0);    //不是Const
    tmp_4.IDENT_if_ret_int = 0;
    Scope.push_back(tmp_4);
    IDENT_scope tmp_5 = IDENT_scope("putch", "0", DEEP, 0);    //不是Const
    tmp_5.IDENT_if_ret_int = 0;
    Scope.push_back(tmp_5);
    IDENT_scope tmp_6 = IDENT_scope("putarray", "0", DEEP, 0);    //不是Const
    tmp_6.IDENT_if_ret_int = 0;
    Scope.push_back(tmp_6);

    yyparse();
    return 0;
}
