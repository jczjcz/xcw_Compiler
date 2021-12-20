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

using namespace std;

void yyerror(const char *);
void yyerror(const string&);
extern int yylex();
extern int yyparse();

ostream &out = cout;       // 用于输出

int VAR_T_num = 0, VAR_t_num = 0, VAR_p_num = 0;       //这三个全局变量分别表示Eeyore中的原生变量、临时变量和函数参数的编号

struct Ptr_num{             // 用来传递参数，用IF_ptr_int表示传上来的是否为常量
    int ptr_int;
    string ptr_str;
    bool IF_ptr_int;
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

struct Params_num{      //函数参数的元素类型，需要包括 INT型变量和INT型数组指针
    string para_str;       //变量名字
    string para_ir_name;    //在IR中，函数参数的名字
};


int DEEP;      //当前的深度
string IF_DEEP(){
    if(DEEP==0)
        return "";
    return "\t";
}



struct IDENT_scope{     //符号表元素
    string IDENT_name;
    string IDENT_num;          // 变量的值可变，因此用string存储
    int  IDENT_const_num;      // const常量直接用INT型数字表示其内容
    int Array_size;
    vector<Ptr_num>* IDENT_array;     // 指向数组头部的指针
    vector<int>* IDENT_dim_array;     // 用于存储数组的维度
    //vector<Params_num>* IDENT_func_param;     // 用一个数组存储所有的参数

    int IDENT_func_param_num;      //参数的个数
    int IDENT_deep;
    bool IDENT_if_const;
    string IR_name;          // 在Eeyore中的变量名

    bool IDENT_if_array;       //是否为数组变量
    bool IDENT_if_func;        //是否为函数变量

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

    void Print_IDENT(){          // 输出所有变量，方便调试
        out << "Name = " << IDENT_name << endl;
        if(IDENT_if_const)
            out << "    Num = " << IDENT_const_num << endl;
        else
            out << "    Num = " << IDENT_num << endl;
        out << "    Deep = " << IDENT_deep << endl;
        out << "    IF_const = " << IDENT_if_const << endl;
        if(!IDENT_if_const)
            out << "    IR_name = " << IR_name << endl;
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
int path_length = 1;
int Array_loc;
int Array_dest, old_Array_dest;    //old_Array_dest用来临时存之前的Array_dest
int Array_deep;
string Array_name;
//----------------------------------------------------------

//-----------------函数相关变量------------------------------


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
    Decl
    | FuncDef
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
        //tmp.Print_IDENT();
        if(check_define(*ToStr($1))){       //如果在当前域中未被定义过
            //out << "11111" << endl;
            Scope.push_back(tmp);
            //VAR_T_num ++ ;
        }
        else{
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
    | IDENT ArrayDef ASSIGN
    {
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
        // 输出 var 24 T0 
        out << "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;

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
            //out << "------- Array_dest = "<<Array_dest<<endl;
            for(; Array_loc < Array_dest; Array_loc++){
                //out << "Array_loc = " << Array_loc << endl;
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                out << ir_name << " = " << 0 << endl;
            }
            VAR_T_num ++;     //定义结束后，把变量名数字 + 1
            Array_dim.clear();     //初始化数组维度
        }
;

ConstInitVal: 
    ConstExp
;

ConstExp:
    AddExp
    {
        $$ = $1;
        //out << "ADD EXP" << endl;
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
        //tmp.Print_IDENT();
        if(check_define(*ToStr($1))){       //如果在当前域中未被定义过
            Scope.push_back(tmp);
            out << IF_DEEP() + "var T" << VAR_T_num << endl;
            out << IF_DEEP() + "T" << VAR_T_num << " = " << 0 << endl;
            VAR_T_num ++ ;
        }
        else{
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
    | IDENT ASSIGN InitVal
    {

        if(ToPtrnum($3)->IF_ptr_int == 1){       //传递的是常量
            int num = ToPtrnum($3)->ptr_int;
            IDENT_scope tmp = IDENT_scope(*ToStr($1), to_string(num), DEEP, 0);  
            tmp.IR_name = "T" + to_string(VAR_T_num);   
            Scope.push_back(tmp);
            out << IF_DEEP() + "var T" << VAR_T_num << endl;
            out << IF_DEEP() + "T" << VAR_T_num << " = " << num << endl;
            VAR_T_num ++ ;
        }
        else{              //传递的是变量
            // out << "in the else " << endl;
            // out << "----------IF_ptr_str = " << ToPtrnum($3)->ptr_str << endl;
            string num = ToPtrnum($3)->ptr_str;

            // out << "------------num = " << num << endl;
            IDENT_scope tmp = IDENT_scope(*ToStr($1), num, DEEP, 0);
            tmp.IR_name = "T" + to_string(VAR_T_num);   
            Scope.push_back(tmp);
            out << IF_DEEP() + "var T" << VAR_T_num << endl;
            out << IF_DEEP() + "T" << VAR_T_num << " = " << num << endl;
            VAR_T_num ++ ;
        }
        
        //tmp.Print_IDENT();
    }
    | IDENT ArrayDef
    {
        //首先检查当前域中是否出现
        if(!check_define(*ToStr($1))){
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }

        // out << "Array num = " << ToPtrnum($2)->ptr_int << endl;
        // yyerror("test");
        int n = ToPtrnum($2)->ptr_int;      //当前数组的元素总数，例如a[2][3], n=6
        // 输出 var 24 T0 
        out << IF_DEEP() + "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;
        
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
            out << IF_DEEP() + ir_name << " = " << 0 << endl;
        }
        VAR_T_num ++;
    }
    | IDENT ArrayDef ASSIGN 
    {
        
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
        // 输出 var 24 T0 
        out << IF_DEEP() + "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;

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
            //out << "------- Array_dest = "<<Array_dest<<endl;
            for(; Array_loc < Array_dest; Array_loc++){
                //out << "Array_loc = " << Array_loc << endl;
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                out << IF_DEEP() + ir_name << " = " << 0 << endl;
            }
            VAR_T_num ++;     //定义结束后，把变量名数字 + 1
            Array_dim.clear();     //初始化数组维度
        }
;

ArrayInit:
    {
        // 类似于{}的情况，推出空值
    }
    | ArrayExps
    {
        //类似于 {1},{1,2}
       // out << "ArrayExps " << endl;
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
        //out << "Array_loc = " << Array_loc << endl;
        //out << "EXP" << endl;
        if(ToPtrnum($1)->IF_ptr_int){    //为常量，加入到数组中
            Ptr_num tmp_ptr = Ptr_num(ToPtrnum($1)->ptr_int);     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
            Scope.back().IDENT_array->push_back(tmp_ptr);
            out << IF_DEEP() + ir_name << " = " << ToPtrnum($1)->ptr_int << endl;
        }       
        else{          //为变量，加入到数组中
            Ptr_num tmp_ptr = Ptr_num(ToPtrnum($1)->ptr_str);     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
            Scope.back().IDENT_array->push_back(tmp_ptr);
            out << IF_DEEP() + ir_name << " = " << ToPtrnum($1)->ptr_str << endl;
        }
        Array_loc ++;     //位置向前进1
    }
    | LCURLY 
    {
        //out << "LCURLY" << endl;
        path_length = path_length / Array_dim[Array_deep];
        Array_deep ++;      //遇到左括号，深度+1
        old_Array_dest = Array_dest;
        Array_dest = Array_loc + path_length;
    }
        ArrayInit RCURLY
        {
            //out << "RCURLY" << endl;
            //out << "------- Array_dest = "<<Array_dest<<endl;
            for(; Array_loc < Array_dest; Array_loc++){
                //out << "Array_loc = " << Array_loc << endl;
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                out << IF_DEEP() + ir_name << " = " << 0 << endl;
            }
            Array_dest = old_Array_dest;
            Array_deep --;      //遇到右括号，深度-1
            path_length = path_length * Array_dim[Array_deep];
        }

;



ArrayDef:
    ArrayUnit
    {
        //out << "Array num1111 = " << ToPtrnum($1)->ptr_int << endl;
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
        //out << "dim = " << ToPtrnum($2)->ptr_int << endl;
        Array_dim.push_back(ToPtrnum($2)->ptr_int);      //把数组数据放到Array_dim中，记录数组维度信息
        
    }
;

InitVal: 
    Exp
    {
        // out << "LVAL end"<<endl;
        // out << "IF_ptr_str = "<<ToPtrnum($1)->ptr_str<<endl;
        // $$ = $1;
    }
;

Exp:
    AddExp
;

AddExp:
    MulExp
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
            string str1, str2;
            if(add_1->IF_ptr_int){
                str1 = to_string(add_1->ptr_int);
            }
            else{
                str1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                str2 = to_string(mul_1->ptr_int);
            }
            else{
                str2 = mul_1->ptr_str;
            }

            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str1 << " + " << str2 << endl;      // 输出类似于 t0 = T0 + 1
        }
        //out << "AddExp ADD MulExp" << *ToInt($1) << "+" << *ToInt($3) << endl;
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
            string str1, str2;
            if(add_1->IF_ptr_int){
                str1 = to_string(add_1->ptr_int);
            }
            else{
                str1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                str2 = to_string(mul_1->ptr_int);
            }
            else{
                str2 = mul_1->ptr_str;
            }

            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str1 << " - " << str2 << endl;      // 输出类似于 t0 = T0 + 1
        }
        $$ = tmp_ptr; 
    }

;

MulExp:
    UnaryExp
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
            string str1, str2;
            if(add_1->IF_ptr_int){
                str1 = to_string(add_1->ptr_int);
            }
            else{
                str1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                str2 = to_string(mul_1->ptr_int);
            }
            else{
                str2 = mul_1->ptr_str;
            }

            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str1 << " * " << str2 << endl;      // 输出类似于 t0 = T0 + 1
        }
        //out << "AddExp ADD MulExp" << *ToInt($1) << "+" << *ToInt($3) << endl;
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
            string str1, str2;
            if(add_1->IF_ptr_int){
                str1 = to_string(add_1->ptr_int);
            }
            else{
                str1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                if(mul_1->ptr_int == 0){       //除0报错
                    yyerror("Integer division by zero.");
                }
                str2 = to_string(mul_1->ptr_int);
            }
            else{
                str2 = mul_1->ptr_str;
            }

            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str1 << " / " << str2 << endl;      // 输出类似于 t0 = T0 + 1
        }
        //out << "AddExp ADD MulExp" << *ToInt($1) << "+" << *ToInt($3) << endl;
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
            string str1, str2;
            if(add_1->IF_ptr_int){
                str1 = to_string(add_1->ptr_int);
            }
            else{
                str1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                if(mul_1->ptr_int == 0){       //除0报错
                    yyerror("Integer mod by zero.");
                }
                str2 = to_string(mul_1->ptr_int);
            }
            else{
                str2 = mul_1->ptr_str;
            }

            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str1 << " % " << str2 << endl;      // 输出类似于 t0 = T0 + 1
        }
        //out << "AddExp ADD MulExp" << *ToInt($1) << "+" << *ToInt($3) << endl;
        $$ = tmp_ptr; 
    }
;

UnaryExp:
    PrimaryExp
    {
        $$ = $1;
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
        // out << "LVAL end"<<endl;
        // out << "IF_ptr_str = "<<ToPtrnum($1)->ptr_str<<endl;
        $$ = $1;
    }
;

LVal:
    IDENT
    {
        IDENT_scope* tmp = find_define(*ToStr($1));
        //out << "Tostr -> " << *ToStr($1) << endl;
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
        Array_name = tmp->IR_name;
        Array_dim.clear();
        for(int i = 0;i < (*tmp).IDENT_dim_array->size();i++){
            Array_dim.push_back((*(*tmp).IDENT_dim_array)[i]);
        }
    }
    ArrayLVals
    {        //a[2][3]     a[1][b]  
        //out << "Left = ArrayLVals " << endl;
        // IDENT_scope* tmp = ((IDENT_scope*)$$); 
        Ptr_num tmp_ptr, tmp_ptr_new, tmp_ptr_old;
        int ptr_size = INTSIZE;

        for(int i = Array_LVal_dim.size()-1; i >= 0 ;i --){
            //out << "i = "<<i << endl;
            // tmp_ptr_old = tmp_ptr_new;
            tmp_ptr = Array_LVal_dim[i];
            //out << "int = " << tmp_ptr.ptr_int << endl;
            if(tmp_ptr.IF_ptr_int){     //是整数
                //out << "tmp_ptr.IF_ptr_int = 1" << endl;
                tmp_ptr_new.IF_ptr_int = 1;
                tmp_ptr_new.ptr_int = tmp_ptr.ptr_int * ptr_size;
                //out << "tmp_ptr_new.ptr_int = " << tmp_ptr_new.ptr_int << endl;
                if(i != Array_LVal_dim.size()-1){     //第一次，不用考虑和之前相加
                    if(tmp_ptr_old.IF_ptr_int){     //如果前面的也是INT
                        tmp_ptr_old.ptr_int += tmp_ptr_old.ptr_int;
                    }
                    else{
                        out << IF_DEEP() + "t" + to_string(VAR_t_num ) << " = "<< tmp_ptr_new.ptr_int << " + " << tmp_ptr_old.ptr_str << endl;
                        tmp_ptr_old.ptr_str = "t" + to_string(VAR_t_num);
                        VAR_t_num ++;
                    }
                }
                else{
                    tmp_ptr_old = tmp_ptr_new;
                }
            }
            else{
                //out << "tmp_ptr.IF_ptr_int = 0" << endl;
                tmp_ptr_new.IF_ptr_int = 0;
                tmp_ptr_new.ptr_str = "t" + to_string(VAR_t_num);
                tmp_ptr_old.ptr_str = to_string(tmp_ptr_old.ptr_int);    //强制转换为string类型
                tmp_ptr_old.IF_ptr_int = 0;
                VAR_t_num ++;
                out << IF_DEEP() + tmp_ptr_new.ptr_str << " = " << tmp_ptr.ptr_str << " * " << ptr_size << endl;
                if(i != Array_LVal_dim.size()-1){     //第一次不用考虑和之前相加
                    //out << "tmp_ptr_old.ptr_int = " << tmp_ptr_old.ptr_int << endl;
                    out <<IF_DEEP() + "t" + to_string(VAR_t_num ) << " = "<< tmp_ptr_new.ptr_str << " + " << tmp_ptr_old.ptr_str << endl;
                    tmp_ptr_old.ptr_str = "t" + to_string(VAR_t_num);
                    VAR_t_num ++;
                }
                else{
                    tmp_ptr_old = tmp_ptr_new;
                }
            }
            ptr_size *= Array_dim[i];
            // out << "test ptr_size" << endl;
            // out << (*(tmp->IDENT_dim_array)) << endl;
            // out << "test ptr_size" << endl;
        }
        Array_LVal_dim.clear();
        //out << "Arrayend " << endl;
        if(tmp_ptr_old.IF_ptr_int){
            tmp_ptr_old.ptr_str = Array_name + "[" + to_string(tmp_ptr_old.ptr_int) + "]";
            tmp_ptr_old.IF_ptr_int = 0;
        }
        else    
            tmp_ptr_old.ptr_str = Array_name + "[" + tmp_ptr_old.ptr_str + "]";
        //out << "tmp_ptr_old.ptr_str = " << tmp_ptr_old.ptr_str << endl; 
        $$ = & tmp_ptr_old;
    }
;

ArrayLVals:
    ArrayLVal
    | ArrayLVals ArrayLVal
;

ArrayLVal:
    LBRAC Exp RBRAC
    {
        Array_LVal_dim.push_back(*(ToPtrnum($2)));    //存入进行引用的维度
    }
;


FuncDef:
    INT IDENT
    {
        //out << " This is a INT Func" << endl;
        //首先检查当前域中是否出现
        if(!check_define(*ToStr($2))){
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
        //  开始函数定义
        IDENT_scope tmp = IDENT_scope(*ToStr($2), "0", DEEP, 0);    //不是Const
        //tmp.IDENT_func_param_num = 0;      //没有参数
        Scope.push_back(tmp);
        
    }
    LPAREN 
    {
        DEEP ++;
    }
    FuncFParams RPAREN
    {
        DEEP --;
        //IDENT_scope* tmp_ptr = find_define(*ToStr($2));    //找到函数变量的指针
        //tmp_ptr->IDENT_func_param_num = 
        out << "f_" << *ToStr($2) << " [" << VAR_p_num << "]" << endl;  
    }
    Block
    {
        
        out << "\treturn 0" << endl;
        out << "end " << "f_" << *ToStr($2) << endl;

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
        IDENT_scope tmp = IDENT_scope(*ToStr($2), "0", DEEP, 0);    //不是Const
        //tmp.IDENT_func_param_num = 0;      //没有参数
        Scope.push_back(tmp);
        //out << "f_" << *ToStr($2) << " [" << tmp.IDENT_func_param_num << "]" << endl;
    }
    LPAREN
    {
        DEEP ++;
    }
    FuncFParams RPAREN
    {
        DEEP --;
        out << "f_" << *ToStr($2) << " [" << VAR_p_num << "]" << endl;
    }
    Block
    {
        
        out << "\treturn" << endl;
        out << "end " << "f_" << *ToStr($2) << endl;

        
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
    {        //a(int b){}
        //out << "INT IDENT" << endl;
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
    | INT IDENT LBRAC RBRAC ArrayParamDef
    {      //void d(int d[])
        // 检查是否出现过
        if(!check_define(*ToStr($2))){
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }

        IDENT_scope tmp = IDENT_scope(*ToStr($2), "0", DEEP, 0);    //是一个变量
        tmp.IDENT_if_array = 1;    //是数组
        tmp.IR_name = "p" + to_string(VAR_p_num);
        //tmp.Print_IDENT();
        VAR_p_num ++;
        Scope.push_back(tmp);
    }
;

ArrayParamDef:
    ArrayUnit
    {
        //out << "Array num1111 = " << ToPtrnum($1)->ptr_int << endl;
        $$ = $1;
    }
    | ArrayParamDef ArrayUnit
    {
        ToPtrnum($1)->ptr_int = ToPtrnum($1)->ptr_int * ToPtrnum($2)->ptr_int;
        $$ = $1;
    }
    |
    {
        //也有可能为空
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
        // out << "BlockItem"<<endl;
        //暂时先考虑内容为空的情况
    }
    | Decl
    {

    }
;




%%

void yyerror(const char *s) {
    extern int yylineno, charNum;
    cout << "Line " << yylineno << "," << charNum << ": " << s << endl;
    exit(1);
}

void yyerror(const string &s) {
    yyerror(s.c_str());
}

int main() {
    ios::sync_with_stdio(false);
    yyparse();
    return 0;
}
