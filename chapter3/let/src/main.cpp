#include <algorithm>
#include <sstream>
#include "lex.yy.h"
#include "interpreter.h"

int main () {

  std::istringstream ss(
      R"EOF(let z = 5
in let x = 3
in let y = -(x,1)
in let x = 4 in
  if zero?(-(z,z))
    then -(z, -(x,y))
    else 0)EOF");

  using eopl::Program, eopl::value_of, eopl::Env;

  yy::Lexer lexer(ss);
  Program result;
  yy::parser p(lexer, result);
  p.set_debug_level(getenv("YYDEBUG") != nullptr);

  try {
    p.parse();
    std::cout << value_of(result, Env::make_empty()) << std::endl;
  } catch (const yy::parser::syntax_error& e) {
    std::cout << "The program came into an error around "
              << e.location << ". Detail: " << e.what()
              << std::endl;
  }
}
