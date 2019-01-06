%option noyywrap

%top {

#include "parser.tab.hh"
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


identifier  [[:alpha:]]([[:alnum:]]|[_])*
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
                  std::cout << loc << std::endl;
                }

"'"             return yy::parser::token::QUOTE;

{blank}         loc.step();
{newline}       loc.lines();
                
{integer}       {
                  std::cout << "integer: " << yytext << " " << loc << std::endl;      
                  yylval.emplace<int>(std::stoi(yytext));            
                  return yy::parser::token::INT;
                }

{s_identifier}  {
                  std::cout << "identifier: " << yytext << " " << loc << std::endl;
                }

{string}        {
                  std::cout << "string: " << yytext << " " << loc << std::endl;
                  yylval.emplace<std::string>(yytext);
                  return yy::parser::token::STRING;
                }
                
{arbitrary}     std::cout << "arbitrary: " << yytext << std::endl; return yytext[0];
                

%%


