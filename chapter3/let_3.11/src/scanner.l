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
s_non_delim   [[:alnum:]]|[!@#$%^&*\-_+=:,<.>/?]
s_legal_start [[:alpha:]]{+}[!@#$%^&*\-_+=:,<.>/?]{-}[#,]
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

"zero?"         return yy::parser::token::ZERO_TEST;

"if"            return yy::parser::token::IF;

"then"          return yy::parser::token::THEN;

"else"          return yy::parser::token::ELSE;

"let"           return yy::parser::token::LET;

"in"            return yy::parser::token::IN;

{blank}         loc.step();
{newline}       loc.lines();

{integer}       {
                  yylval.emplace<eopl::Int>(std::stoi(yytext));
                  // std::cout << "integer: " << yytext << std::endl;
                  return yy::parser::token::INT;
                }

{identifier}  {
                  yylval.emplace<eopl::Symbol>(yytext);
                  // std::cout << "identifier: " << yytext << std::endl;
                  return yy::parser::token::IDENTIFIER;
                }
{arbitrary}     return yytext[0];

%%


