#include <algorithm>
#include <sstream>
#include "lex.yy.h"
#include "interpreter.h"

int main () {

  std::istringstream ss(
      R"EOF(let z = 5
in let x = 3
in let y = -(x,1) % here x = 3
in let x = 4 in -(z, -(x,y)) % here x = 4)EOF");

  using eopl::Program, eopl::value_of, eopl::Env;

  yy::Lexer lexer(ss);
  Program result;
  yy::parser p(lexer, result);
  p.set_debug_level(getenv("YYDEBUG") != nullptr);

  try {
    p.parse();
    std::cout << value_of(result, Env::make_empty()) << std::endl;
  } catch (const yy::parser::syntax_error& e) {
    std::cout << "The program came into " << e.what()
              << " around " << e.location << std::endl;
  }
}
