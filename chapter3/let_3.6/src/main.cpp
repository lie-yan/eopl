#include <algorithm>
#include <sstream>
#include "lex.yy.h"
#include "interpreter.h"

namespace eopl {

Value eval (const std::string& s) {
  std::istringstream ss(s);
  yy::Lexer lexer(ss);
  Program result;
  yy::parser p(lexer, result);
  p.set_debug_level(getenv("YYDEBUG") != nullptr);
  p.parse();
  return value_of(result, Env::make_empty());
}

}

int main () {
  using eopl::eval, eopl::Int, eopl::Value;
  try {
    assert(eval(R"EOF(
let z = 5
in let x = 3
in let y = -(x,1)
in let x = 4 in -(z, -(x,y)))EOF") == Value(Int{3}));

    assert(eval(R"EOF(
let z = 5
in let x = 3
in if zero?(-(z,-(x,-2))) then 5 else 3)EOF") == Value(Int{5}));

    assert(eval(R"EOF(
let z = -5
in let x = 3
in if zero?(-(minus(z),-(x,-2))) then 5 else 3)EOF") == Value(Int{5}));

    assert(eval(R"EOF(
let z = -5
in let x = 3
in if zero?(-(minus(z),-(x, 2))) then 5 else 3)EOF") == Value(Int{3}));

  } catch (const yy::parser::syntax_error& e) {
    std::cout << "The program came into an error around "
              << e.location << ". Detail: " << e.what()
              << std::endl;
  }
}
