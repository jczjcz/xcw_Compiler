%{
#define YYSTYPE void*
#define INTSIZE 4

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>

using namespace std;

void yyerror(const char *);
void yyerror(const string&);
extern int yylex();
extern int yyparse();

ostream &out = cout;       // 用于输出





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
    CompUnits Decl
    |   Decl
;

CompUnits:
    CompUnit CompUnits
    |   CompUnit
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
    VarDef VarDefs
    |   VarDef
;

VarDef:
    IDENT
    {
        $$ = $1;
        cout << "hello world" << endl;
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
