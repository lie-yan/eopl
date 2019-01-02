%{
#include <string>
#include <iostream>
#include <iomanip>
#include <memory>

const int common_width = 12;
%}

%option debug
%option c++
%option noyywrap

letters     [a-zA-Z]
digits      [0-9]

string      \"([^\\\"]|\\.)*\"

identifier  {letters}({letters}|{digits}|[_.\-/$])*
number      {digits}+
whitespace  [ \t]+

%%
{whitespace}  /* skip whitespace */

"\n"          {
                std::cout << std::setw(common_width) << "newline  " << std::endl;
              }

"%"           { // skip comment
                for (int c = yyinput(); c != '\n'; c = yyinput());
              }
{number}      {
                std::cout << std::setw(common_width) << "number: " << YYText() << std::endl; 
              }
{identifier}  {
                std::cout << std::setw(common_width) << "identifier: " << YYText() << std::endl;
              }
{string}      {
                std::cout << std::setw(common_width) << "string: " << YYText() << std::endl;
              }

%%

int main(int argc, char** argv) {
  std::unique_ptr<yyFlexLexer> lexer = std::make_unique<yyFlexLexer>();
  while (lexer->yylex() != 0)
    ;
}