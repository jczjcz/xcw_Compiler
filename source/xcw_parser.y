%{
#define YYSTYPE void*
#define INTSIZE 4
#define ToStr(k) (string*)(k)
#define ToInt(k) (int*)(k)

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



int DEEP;      //当前的深度
struct IDENT_scope{     //符号表元素
    string IDENT_name;
    int IDENT_num;
    int IDENT_deep;
    IDENT_scope(string name, int num, int deep){
        IDENT_name = name;
        IDENT_num = num;
        IDENT_deep = deep;
    }
};

vector<IDENT_scope> Scope;     //符号表

bool check_define(IDENT_scope ident){       // 检查当前域中是否存在重复
    int i = Scope.size() - 1;
    if(i == -1)
        return true;
    while(Scope[i].IDENT_deep == DEEP){
        if(ident.IDENT_name == Scope[i].IDENT_name){
            return false;
        }
        i--;
    }
    return true;
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
    VarDecl
;

VarDecl:
    BType VarDefs SEMI
;

BType:
    INT
;

VarDefs:
    VarDef
    |   VarDefs VarDef
;

VarDef:
    IDENT
    {
        $$ = $1;
        IDENT_scope tmp = IDENT_scope(*ToStr($1), 0, DEEP);
        if(check_define(tmp)){       //如果在当前域中未被定义过
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
