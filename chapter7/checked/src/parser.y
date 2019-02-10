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

using eopl::ConstExp, eopl::VarExp, eopl::IfExp, eopl::LetExp, eopl::UnpackExp;
using eopl::CondExp, eopl::ProcExp, eopl::CallExp, eopl::LetrecExp, eopl::Program;
using eopl::LetrecProc;
using eopl::to_exp;

}

%token <eopl::Int>    INT           "integer"
%token <eopl::Symbol> IDENTIFIER    "identifier"
%token                IF            "if"
%token                THEN          "then"
%token                ELSE          "else"
%token                LET           "let"
%token                LET_STAR      "let*"
%token                LETREC        "letrec"
%token                UNPACK        "unpack"
%token                IN            "in"
%token                COND          "cond"
%token                RIGHT_ARROW   "==>"
%token                END           "end"
%token                PROC          "proc"
%token                END_OF_FILE   "end of file"
%token                BOOL_TYPE     "bool"
%token                INT_TYPE      "int"

%type <eopl::Expression> expression
%type <std::vector<eopl::Expression>> exp_nlist
%type <eopl::CondExp::ClauseList> cond_clause_list
%type <eopl::CondExp::Clause> cond_clause
%type <eopl::LetExp::ClauseList> let_clause_list
%type <eopl::LetExp::Clause> let_clause
%type <bool> let_variant
%type <std::vector<eopl::Symbol>> id_list
%type <std::vector<eopl::Symbol>> param_list
%type <eopl::LetrecProc> proc_spec
%type <std::vector<eopl::LetrecProc>> proc_spec_nlist
%%

program : expression { result = Program{std::move($1)}; }
        ;

expression  : INT      { $$ = to_exp(ConstExp{$1}); }
            | IDENTIFIER { $$ = to_exp(VarExp{$1}); }
            | IF expression THEN expression ELSE expression
              { $$ = to_exp(IfExp{std::move($2), std::move($4), std::move($6)}); }
            | COND cond_clause_list END
              { $$ = to_exp(CondExp{std::move($2)}); }
            | let_variant let_clause_list IN expression
              { $$ = to_exp(LetExp{std::move($2), std::move($4), $1}); }
            | UNPACK id_list '=' expression IN expression
              { $$ = to_exp(UnpackExp{std::move($2), std::move($4), std::move($6)}); }
            | PROC '(' param_list ')' expression
              { $$ = to_exp(ProcExp{std::move($3), std::move($5)}); }
            | '(' expression exp_nlist ')'
              { $$ = to_exp(CallExp{std::move($2), std::move($3)}); }
            | LETREC proc_spec_nlist IN expression
              { $$ = to_exp(LetrecExp{std::move($2), std::move($4)}); }
            ;

exp_nlist : expression  { $$ = {std::move($1)}; }
          | exp_nlist expression { $1.push_back(std::move($2)); $$ = std::move($1); }
          ;

cond_clause_list : %empty { $$ = eopl::CondExp::ClauseList(); }
                 | cond_clause_list cond_clause { $1.push_back(std::move($2)); $$ = std::move($1); }
                 ;

cond_clause : expression RIGHT_ARROW expression
              { $$ = eopl::CondExp::Clause{std::move($1), std::move($3)}; }
            ;

let_clause_list : %empty { $$ = eopl::LetExp::ClauseList(); }
                | let_clause_list let_clause { $1.push_back(std::move($2)); $$ = std::move($1); }
                ;

let_clause : IDENTIFIER '=' expression
             { $$ = eopl::LetExp::Clause{std::move($1), std::move($3)}; }
           ;

let_variant : LET { $$ = false; }
            | LET_STAR { $$ = true; }
            ;

proc_spec_nlist : proc_spec { $$ = std::vector<eopl::LetrecProc>{std::move($1)}; }
               | proc_spec_nlist proc_spec { $1.push_back(std::move($2)); $$ = std::move($1); }
               ;

proc_spec : IDENTIFIER '(' param_list ')' '=' expression
           { $$ = LetrecProc{std::move($1), std::move($3), std::move($6)}; }
         ;

id_list : %empty { $$ = std::vector<eopl::Symbol>(); }
        | id_list IDENTIFIER { $1.push_back(std::move($2)); $$ = std::move($1); }
        ;

param_list : %empty { $$ = std::vector<eopl::Symbol>(); }
           | IDENTIFIER { $$ = std::vector<eopl::Symbol>{std::move($1)}; }
           | param_list ',' IDENTIFIER { $1.push_back(std::move($3)); $$ = std::move($1); }
           ;

%%
