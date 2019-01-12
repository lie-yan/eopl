#include <algorithm>
#include <sstream>
#include <fmt/format.h>
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

void print_test_success(int i) {
  fmt::print("Passed test case {}.\n", i);
}

int main () {
  using eopl::eval, eopl::Int, eopl::Value;
  try {
    assert(eval(R"EOF(
let z = 5
in let x = 3
in let y = -(x,1)
in let x = 4 in -(z, -(x,y)))EOF") == Value(Int{3}));
    int i = 0;
    print_test_success(i++);

    assert(eval(R"EOF(
let z = 5
in let x = 3
in if zero?(-(z,-(x,-2))) then 5 else 3)EOF") == Value(Int{5}));
    print_test_success(i++);

  } catch (const yy::parser::syntax_error& e) {
    fmt::print("The program came into an error around {}. Detail: {}.\n", e.location, e.what());
  } catch (const std::runtime_error& e) {
    fmt::print("The program came into a runtime error: {}.\n", e.what());
  }
}
