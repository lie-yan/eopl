#! /bin/sh

reflex --flex --bison-cc --bison-locations \
  --namespace=yy --lexer=Lexer --lex=yylex \
  --header-file=lex.yy.h --interactive scanner.l
bison -d parser.y

# c++ -std=c++17 main.cpp lex.yy.cpp parser.tab.cc -lreflex

