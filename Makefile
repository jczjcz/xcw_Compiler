.PHONY: lc
lc:
	lex -o build/xcw_main.l.yy.cpp source/xcw_scanner.l
	yacc -v --defines=build/xcw_parser.tab.h --output=build/xcw_parser.tab.c source/xcw_parser.y
	g++ -o build/compiler -I source build/xcw_main.l.yy.cpp build/xcw_parser.tab.c