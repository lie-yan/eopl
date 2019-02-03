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
#include "stmt.h"

namespace yy {
  class Lexer;  // Generated by reflex with namespace=yy lexer=Lexer lex=yylex
  using Result = eopl::Program;
}

const int star_mask = 0b01;
const int mutable_mask = 0b10;
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

using eopl::ConstExp;
using eopl::VarExp;
using eopl::IfExp;
using eopl::LetExp;
using eopl::BindingClause;
using eopl::BindingClauseList;
using eopl::UnpackExp;
using eopl::CondExp;
using eopl::ProcExp;
using eopl::CallExp;
using eopl::LetrecExp;
using eopl::LetrecProcSpec;
using eopl::SequenceExp;
using eopl::AssignExp;
using eopl::SetdynamicExp;
using eopl::Statement;
using eopl::AssignStmt;
using eopl::SubrCallStmt;
using eopl::BlockStmt;
using eopl::IfStmt;
using eopl::WhileStmt;
using eopl::DeclStmt;
using eopl::Program;
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
%token                LETMUTABLE    "letmutable"
%token                UNPACK        "unpack"
%token                SET           "set"
%token                IN            "in"
%token                COND          "cond"
%token                RIGHT_ARROW   "==>"
%token                BEGIN_        "begin"
%token                END           "end"
%token                PROC          "proc"
%token                SETDYNAMIC    "setdynamic"
%token                DURING        "during"
%token                VAR           "var"
%token                WHILE         "while"
%token                END_OF_FILE   "end of file"


%type <eopl::Statement> statement
%type <eopl::Expression> expression
%type <std::vector<eopl::Expression>> exp_nlist
%type <std::vector<eopl::Expression>> semicol_sep_exp_nlist
%type <std::vector<eopl::Statement>> stmt_list
%type <eopl::CondExp::ClauseList> cond_clause_list
%type <eopl::CondExp::Clause> cond_clause
%type <eopl::BindingClauseList> assign_clause_list
%type <eopl::BindingClause> assign_clause
%type <int> let_variant
%type <std::vector<eopl::Symbol>> id_list
%type <std::vector<eopl::Symbol>> comma_sep_id_list
%type <eopl::LetrecProcSpec> proc_spec
%type <std::vector<eopl::LetrecProcSpec>> proc_spec_nlist
%%

program : statement { result = {std::move($1)}; }
        ;

statement : IDENTIFIER '=' expression
            { $$ = to_stmt(AssignStmt{std::move($1), std::move($3)}); }
          | '(' IDENTIFIER exp_nlist ')'
            { $$ = to_stmt(SubrCallStmt{std::move($2), std::move($3)}); }
          | '{' stmt_list '}'
            { $$ = to_stmt(BlockStmt{std::move($2)}); }
          | IF expression statement statement
            { $$ = to_stmt(IfStmt{std::move($2), std::move($3), std::move($4)}); }
          | WHILE expression statement
            { $$ = to_stmt(WhileStmt{std::move($2), std::move($3)}); }
          | VAR comma_sep_id_list ';' statement
            { $$ = to_stmt(DeclStmt{std::move($2), std::move($4)}); }
          ;

expression  : INT      { $$ = to_exp(ConstExp{$1}); }
            | IDENTIFIER { $$ = to_exp(VarExp{$1}); }
            | IF expression THEN expression ELSE expression
              { $$ = to_exp(IfExp{std::move($2), std::move($4), std::move($6)}); }
            | COND cond_clause_list END
              { $$ = to_exp(CondExp{std::move($2)}); }
            | let_variant assign_clause_list IN expression
              { $$ = to_exp(LetExp{std::move($2), std::move($4), bool($1 & star_mask), bool(mutable_mask)}); }
            | UNPACK id_list '=' expression IN expression
              { $$ = to_exp(UnpackExp{std::move($2), std::move($4), std::move($6)}); }
            | PROC '(' comma_sep_id_list ')' expression
              { $$ = to_exp(ProcExp{std::move($3), std::move($5)}); }
            | '(' expression exp_nlist ')'
              { $$ = to_exp(CallExp{std::move($2), std::move($3)}); }
            | LETREC proc_spec_nlist IN expression
              { $$ = to_exp(LetrecExp{std::move($2), std::move($4)}); }
            | BEGIN_ semicol_sep_exp_nlist END
              { $$ = to_exp(SequenceExp{std::move($2)}); }
            | SET IDENTIFIER '=' expression
              { $$ = to_exp(AssignExp{std::move($2), std::move($4)}); }
            | SETDYNAMIC assign_clause_list DURING expression
              { $$ = to_exp(SetdynamicExp{std::move($2), std::move($4)}); }
            ;

stmt_list : %empty { $$ = std::vector<Statement>(); }
          | statement
            { $$ = std::vector<Statement>{std::move($1)}; }
          | stmt_list ';' statement
            { $1.push_back(std::move($3)); $$ = std::move($1); }
          ;

exp_nlist : expression  { $$ = {std::move($1)}; }
          | exp_nlist expression { $1.push_back(std::move($2)); $$ = std::move($1); }
          ;

semicol_sep_exp_nlist : expression { $$ = {std::move($1)}; }
                      | semicol_sep_exp_nlist ';' expression { $1.push_back(std::move($3)); $$ = std::move($1); }
                      ;

cond_clause_list : %empty { $$ = eopl::CondExp::ClauseList(); }
                 | cond_clause_list cond_clause { $1.push_back(std::move($2)); $$ = std::move($1); }
                 ;

cond_clause : expression RIGHT_ARROW expression
              { $$ = eopl::CondExp::Clause{std::move($1), std::move($3)}; }
            ;

assign_clause_list : %empty { $$ = eopl::BindingClauseList(); }
                   | assign_clause_list assign_clause { $1.push_back(std::move($2)); $$ = std::move($1); }
                   ;

assign_clause : IDENTIFIER '=' expression
                { $$ = eopl::BindingClause{std::move($1), std::move($3)}; }
              ;

let_variant : LET { $$ = 0; }
            | LET_STAR { $$ = star_mask; }
            | LETMUTABLE { $$ = mutable_mask; }
            ;

proc_spec_nlist : proc_spec { $$ = std::vector<eopl::LetrecProcSpec>{std::move($1)}; }
                | proc_spec_nlist proc_spec { $1.push_back(std::move($2)); $$ = std::move($1); }
                ;

proc_spec : IDENTIFIER '(' comma_sep_id_list ')' '=' expression
           { $$ = LetrecProcSpec{std::move($1), std::move($3), std::move($6)}; }
          ;

id_list : %empty { $$ = std::vector<eopl::Symbol>(); }
        | id_list IDENTIFIER { $1.push_back(std::move($2)); $$ = std::move($1); }
        ;

comma_sep_id_list : %empty { $$ = std::vector<eopl::Symbol>(); }
                  | IDENTIFIER { $$ = std::vector<eopl::Symbol>{std::move($1)}; }
                  | comma_sep_id_list ',' IDENTIFIER { $1.push_back(std::move($3)); $$ = std::move($1); }
                  ;

%%
