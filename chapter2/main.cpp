#include <iostream>
#include "scope_support.h"
#include <string>

int main () {
  using namespace std::string_literals;
  using eopl::Symbol, eopl::String, eopl::Value, eopl::ValueList;

  std::cout << "sizeof(eopl::Value): " << sizeof(eopl::Value) << std::endl;

  Value v = ValueList{{false, 2, 3.0, Symbol("w"s)}};
  std::cout << v.index() << std::endl;
  ValueList& vlist = std::get<5>(v).get();
  for (auto& x : vlist) {
    std::cout << x.index() << std::endl;
  }
}