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
};

int DEEP;      //当前的深度
struct IDENT_scope{     //符号表元素
    string IDENT_name;
    string IDENT_num;          // 变量的值可变，因此用string存储
    int  IDENT_const_num;      // const常量直接用INT型数字表示其内容
    int IDENT_deep;
    bool IDENT_if_const;
    string IR_name;          // 在Eeyore中的变量名
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
    while(Scope[i].IDENT_deep == DEEP && i >= 0){
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
    Decl
    | CompUnit Decl
;

Decl:
    ConstDecl
    | VarDecl
;

ConstDecl:
    CONST BType ConstDefs SEMI
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
            VAR_T_num ++ ;
        }
        else{
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
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
    BType VarDefs SEMI
;

BType:
    INT
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
            out << "var T" << VAR_T_num << endl;
            out << "T" << VAR_T_num << " = " << 0 << endl;
            VAR_T_num ++ ;
        }
        else{
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
    | IDENT ASSIGN InitVal
    {
        if(!check_define(*ToStr($1))){   
            string err = "\"" +  *ToStr($1) + "\" already defined in this scope.";
            yyerror(err);
        }

        if(ToPtrnum($3)->IF_ptr_int){       //传递的是常量
            int num = ToPtrnum($3)->ptr_int;
            IDENT_scope tmp = IDENT_scope(*ToStr($1), to_string(num), DEEP, 0);  
            tmp.IR_name = "T" + to_string(VAR_T_num);   
            Scope.push_back(tmp);
            out << "var T" << VAR_T_num << endl;
            out << "T" << VAR_T_num << " = " << num << endl;
            VAR_T_num ++ ;
        }
        else{              //传递的是变量
            string num = ToPtrnum($3)->ptr_str;
            //out << "------------num = " << num << endl;
            IDENT_scope tmp = IDENT_scope(*ToStr($1), num, DEEP, 0);
            tmp.IR_name = "T" + to_string(VAR_T_num);   
            Scope.push_back(tmp);
            out << "var T" << VAR_T_num << endl;
            out << "T" << VAR_T_num << " = " << num << endl;
            VAR_T_num ++ ;
        }
        
        //tmp.Print_IDENT();
    }
;

InitVal: 
    Exp
;

Exp:
    AddExp
;

AddExp:
    MulExp
    | AddExp ADD MulExp
    {
        add_1 = ToPtrnum($1);
        mul_1 = ToPtrnum($3);
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            tmp_ptr->ptr_int = add_1->ptr_int + mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            tmp_ptr->ptr_int = add_1->ptr_str + "+" + mul_1->ptr_str;
            tmp_ptr->IF_ptr_int = 0;
            //TODO:生成临时变量
        }
        //out << "AddExp ADD MulExp" << *ToInt($1) << "+" << *ToInt($3) << endl;
        $$ = tmp_ptr; 
    }
    | AddExp SUB MulExp
    {
        int res = *ToInt($1) - *ToInt($3);
        $$ = & res; 
    }

;

MulExp:
    UnaryExp
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
