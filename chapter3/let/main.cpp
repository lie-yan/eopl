#include <algorithm>
#include "lex.yy.h"
#include "parser.tab.hh"

int main() {
  yy::Lexer lexer(std::cin);
  yy::Value result;
  yy::parser p(lexer, result);
  p.set_debug_level(!!getenv("YYDEBUG"));
  
  try {
    p.parse();
    std::for_each(std::begin(result), std::end(result), 
                  [](const auto& x) {
                    std::cout << x << " ";
                  });
    std::cout << std::endl;
  } catch (const yy::parser::syntax_error& e) {
    std::cout << "The program came into " << e.what() 
              << " around " << e.location << std::endl;
  }
}
