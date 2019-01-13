%option noyywrap
%option flex bison-cc bison-locations
%option namespace=yy lexer=Lexer lex=yylex
%option interactive

%top {
#include "parser.tab.hpp"
#include "location.hh"
%}

%class {
  yy::location loc;
%}

%{
#include <iostream>
#include <string>

#define yyterminate() return 0
#undef YY_USER_ACTION
#define YY_USER_ACTION  loc.columns (yyleng);
%}


identifier  [[:alpha:]]([[:alnum:]]|[_?])*
string      \"([^\\\"]|\\\")*\"
newline     \n
blank       [\t ]
whitespace  {newline}|{blank}
arbitrary   .

bracket    [{}\[\]]

s_delimiter   [();"'`|]|{bracket}
s_non_delim   [[:alnum:]]|[!@#$%^&*\-_+=:<.>/?]
s_legal_start [[:alpha:]]{+}[!@#$%^&*\-_+:,<.>/?]{-}[#,]
s_identifier  {s_legal_start}{s_non_delim}*

integer [-+]?[[:digit:]]+

%%

%{ 
  // called on rule match before action
  loc.step();
%}


"%"             {
                  for (int c = yyinput(); c != '\n' && c != 0; c = yyinput())
                    ;
                  loc.lines();
                }

"if"            {
                #ifdef DEBUG
                  std::cout << "keyword: " << yytext << std::endl;
                #endif
                  return yy::parser::token::IF;
                }

"then"          {
                #ifdef DEBUG
                  std::cout << "keyword: " << yytext << std::endl;
                #endif
                  return yy::parser::token::THEN;
                }

"else"          {
                #ifdef DEBUG
                  std::cout << "keyword: " << yytext << std::endl;
                #endif
                  return yy::parser::token::ELSE;
                }

"let"           {
                #ifdef DEBUG
                  std::cout << "keyword: " << yytext << std::endl;
                #endif
                  return yy::parser::token::LET;
                }

"let*"          {
                #ifdef DEBUG
                  std::cout << "keyword: " << yytext << std::endl;
                #endif
                  return yy::parser::token::LET_STAR;
                }

"in"            {
                #ifdef DEBUG
                  std::cout << "keyword: " << yytext << std::endl;
                #endif
                  return yy::parser::token::IN;
                }

"cond"          {
                #ifdef DEBUG
                  std::cout << "keyword: " << yytext << std::endl;
                #endif
                  return yy::parser::token::COND;
                }

"==>"           {
                #ifdef DEBUG
                  std::cout << "keyword: " << yytext << std::endl;
                #endif
                  return yy::parser::token::RIGHT_ARROW;
                }

"end"           {
                #ifdef DEBUG
                  std::cout << "keyword: " << yytext << std::endl;
                #endif
                  return yy::parser::token::END;
                }


{blank}         loc.step();

{newline}       loc.lines();

{integer}       {
                  yylval.emplace<eopl::Int>(std::stoi(yytext));
                #ifdef DEBUG
                    std::cout << "integer: " << yytext << std::endl;
                #endif
                  return yy::parser::token::INT;
                }

{s_identifier}  {
                  yylval.emplace<eopl::Symbol>(yytext);
                #ifdef DEBUG
                  std::cout << "identifier: " << yytext << std::endl;
                #endif
                  return yy::parser::token::IDENTIFIER;
                }

{arbitrary}     {
                #ifdef DEBUG
                  std::cout << "arbitrary: " << yytext << std::endl;
                #endif
                  return yytext[0];
                }

%%


