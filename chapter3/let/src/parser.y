%require "3.2.2"
%language "c++"
%debug

%defines
%define parse.assert
%define api.value.type variant
%locations

%code requires // *.h
{
#include "expr.h"

namespace yy {
  class Lexer;  // Generated by reflex with namespace=yy lexer=Lexer lex=yylex
  using Result = eopl::Program;
}

}

// Construct parser object with lexer and result
%parse-param {yy::Lexer& lexer}{Result& result}

%code // *.cpp
{

#include "lex.yy.h"
#undef yylex
#define yylex lexer.yylex  // Within bison's parse() we should invoke lexer.yylex(), not the global yylex()

// customize the error behavior
void yy::parser::error(const parser::location_type& l, const std::string& m) {
  throw parser::syntax_error(l, m);
}

}

%token <int>          INT           "integer"
%token <eopl::Symbol> IDENTIFIER    "identifier"
%token                ZEROQ         "zero?"
%token                IF            "if"
%token                THEN          "then"
%token                ELSE          "else"
%token                LET           "let"
%token                IN            "in"
%token                END_OF_FILE   "end of file"

%type <eopl::Expression> expression

%%

program : expression { result = eopl::Program{std::move($1)}; }
        ;

expression  : INT      { $$ = eopl::ConstExp{$1}; }
            | '-' '(' expression ',' expression ')'
              { $$ = eopl::DiffExp{std::move($3), std::move($5)}; }
            | ZEROQ '(' expression ')' { $$ = eopl::ZeroQExp{std::move($3)}; }
            | IF expression THEN expression ELSE expression
              { $$ = eopl::IfExp{std::move($2), std::move($4), std::move($6)}; }
            | IDENTIFIER { $$ = eopl::VarExp{$1}; }
            | LET IDENTIFIER '=' expression IN expression
              { $$ = eopl::LetExp{std::move($2), std::move($4), std::move($6)}; }
            ;

%%
