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
  using eopl::eval, eopl::Int, eopl::Bool, eopl::Value;
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
in let true = zero?(0)
in let false = zero?(1)
in if zero?(-(z,-(x,-2))) then true else false )EOF") == Value(Bool{true}));
    print_test_success(i++);

    assert(eval(R"EOF(
let z = 5
in let x = 3
in let x = +(z,x)
in let x = *(x, 10)
in /(x, 5)  )EOF") == Value(Int{16}));
    print_test_success(i++);

  } catch (const yy::parser::syntax_error& e) {
    fmt::print("The program came into an error around {}. Detail: {}.\n", e.location, e.what());
  } catch (const std::runtime_error& e) {
    fmt::print("The program came into a runtime error: {}.\n", e.what());
  }
}
