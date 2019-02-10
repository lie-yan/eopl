#include <iostream>
#include <fmt/ostream.h>
#include "interpreter.h"

int main () {
  using eopl::eval, eopl::Int, eopl::Bool, eopl::Value;
  try {
    auto res = eval("let f = proc (x) (- x 11) in (f (f 77))");
    std::cout << res << std::endl;

    res = eval("(proc (f) (f (f 77)) proc (x) (- x 11))");
    std::cout << res << std::endl;
  } catch (const yy::parser::syntax_error& e) {
    fmt::print("The program came into an error around {}. Detail: {}.\n", e.location, e.what());
  } catch (const std::runtime_error& e) {
    fmt::print("The program came into a runtime error: {}.\n", e.what());
  }
}
