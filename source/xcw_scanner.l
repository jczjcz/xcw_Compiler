%{
#define YYSTYPE void*
#include <iostream>
#include <string>
using namespace std;
#include "xcw_parser.tab.h"
%}
%option yylineno

letter_ [A-Za-z_]
digit [0-9]
octdigit [0-7]
hexdigit [0-9a-fA-F]

identifier [a-zA-Z_][a-zA-Z0-9_]*

decnumber [1-9][0-9]*
octnumber 0[0-7]*
hexnumber (0x|0X)[0-9a-fA-F]+

%%
[\040\t]    {}
[\n\r]  {}
if      { return IF;}
else    { return ELSE;}
continue  { return CONT;}
break   { return BREAK;}
while   { return WHILE;}
return  { return RETURN;}
int     { return INT;}
void    { return VOID;}
const   { return CONST;}
"+"     { return ADD;}
"-"     { return SUB;}
"*"     { return MUL;}
"/"     { return DIV;}
"<"     { return LE;}
"<="    { return LEQ;}
">"     { return GE;}
">="    { return GEQ;}
"=="    { return EQ;}
"!="    { return NEQ;}
"("     { return LPAREN;}
")"     { return RPAREN;}
"{"     { return LCURLY;}
"}"     { return RCURLY;}
"="     { return ASSIGN;}
";"     { return SEMI;}
"["     { return LBRAC;}
"]"     { return RBRAC;}
"&&"    { return AND;}
"||"    { return OR;}
","     { return COMMA;}
"."     { return PERIOD;}
"!"     { return NOT;}
"%"     { return MOD;}


{decnumber} {
    yylval = new int(strtol(yytext, nullptr, 10));
    return NUMBER;
}
{octnumber} {
    yylval = new int(strtol(yytext, nullptr, 8));
    return NUMBER;
}
{hexnumber} {
    yylval = new int(strtol(yytext, nullptr, 16));
    return NUMBER;
}
{identifier} {
    yylval = new string(yytext);
    return IDENT;
}

"//".*$ {/*ignore*/}
"/*"[^"*"]*(\*+[^"*/"][^"*"]*)*\*+"/"  {/*ignore*/}
%%

int yywrap() {
    return 1;
}