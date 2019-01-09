#include <algorithm>
#include "lex.yy.h"
#include "expr.h"

int main () {
  yy::Lexer lexer(std::cin);
  eopl::Program result;
  yy::parser p(lexer, result);
  p.set_debug_level(!!getenv("YYDEBUG"));

  try {
    p.parse();
    std::cout << result << std::endl;
  } catch (const yy::parser::syntax_error& e) {
    std::cout << "The program came into " << e.what()
              << " around " << e.location << std::endl;
  }
}
