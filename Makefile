.PHONY: lc
lc:
	lex -o build/main.l.yy.cpp source/scanner.l
	yacc -v --defines=build/parser.tab.h --output=build/parser.tab.c source/parser.y
	g++ -o build/compiler -I source build/main.l.yy.cpp build/parser.tab.c source/tokenclass.cpp source/tokenclass.h