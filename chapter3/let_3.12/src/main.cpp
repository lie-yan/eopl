#include <algorithm>
#include <sstream>
#include <fmt/format.h>
#include "parser.tab.hpp"
#include "interpreter.h"

void print_test_success(int i) {
  fmt::print("Passed test case {}.\n", i);
}

int main () {
  using eopl::eval, eopl::Int, eopl::Bool, eopl::Value;
  try {
    auto res = eval(R"EOF(
let x = 4 in cons(x,
cons(cons(-(x,1), emptylist), emptylist))
)EOF");
    std::cout << res << std::endl;

    res = eval(R"EOF(
let x = 4 in list(x, -(x,1), -(x,3))
)EOF");
    std::cout << res << std::endl;

    res = eval(R"EOF(
let x = 10
in cond zero?(x) ==> +(x,1)
        greater?(x, 3) ==> +(x,2)
        less?(x, 100) ==> +(x,3)
   end
)EOF");
    std::cout << res << std::endl;
  } catch (const yy::parser::syntax_error& e) {
    fmt::print("The program came into an error around {}. Detail: {}.\n", e.location, e.what());
  } catch (const std::runtime_error& e) {
    fmt::print("The program came into a runtime error: {}.\n", e.what());
  }
}
