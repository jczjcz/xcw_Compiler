%option yylineno
%top{
    #define YYSTYPE void*
    #include<math.h>
    #include <iostream>
    #include <string>
    using namespacr std;

    #include "xcw_parser.tab.h"
}
%{
    // int id_num=0;
    // char id_list[100][20];
%}



commentbegin "/*"
commentelement .|\n
commentend "*/"
comment_1begin "//"
comment_1element .
comment_1end "\n"
%x COMMENT
%x COMMENT1



"0[0-7]+" {return num_base8}
"-?[0-9]+" {return num_base10}
"(0x|0X)[0-9a-fA-F]+" {return num_base16}

num_base10 -?[0-9]+
num_base8 0[0-7]+





%%

[ \r\n\t] { /* ignore */ }


{commentbegin} {BEGIN COMMENT;}
<COMMENT>{commentelement} {comments++;}
<COMMENT>{commentend} {BEGIN INITIAL;} 
{comment_1begin} {BEGIN COMMENT1;}
<COMMENT1>{comment_1element} {comments++;}
<COMMENT1>{comment_1end} {BEGIN INITIAL;} 

int {return INT;}
const {return CONST;}
void {return VOID;}

if {return IF;}
else {return ELSE;}
while {return WHILE;}
break {return BREAK;}
continue {return CONT;}
return {return RETURN;}

"(" {return LPAREN;}
")" {return RPAREN;}
"{" {return LCURLY;}
"}" {return RCURLY;}
"[" {return LBRAC;}
"]" {return RBRAC;}
";" {return SEMI;}


"&&" {return AND;}
"||" {return OR;}
"==" {return EQ;}
"!=" {return NEQ;}
"<=" {return LE;}
">=" {return GE;}

"=" {return ASSIGN;}
"+" {return ADD;}
"-" {return SUB;}
"*" {return MUL;}
"/" {return DIV;}
"%" {return MOD;}
"!" {return NOT;}

"[a-zA-Z_][a-zA-Z0-9_]*" {return IDENT;}







%%


int main(int argc, char **argv){
    if(argc>1){
        yyin = fopen(argv[1],"r");
        if(argc>2){
            yyout = fopen(argv[2],"w");
        }
    }
    yylex();
    // fprintf(yyout, "%8d%8d%8d%8d\n",lines,words,chars,comments);
    // printf("%d\n",yylineno);
    return 0;
}

