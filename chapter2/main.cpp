#include <iostream>
#include "scope_support.h"
#include <string>

int main () {
  using namespace std::string_literals;
  using eopl::Symbol, eopl::String, eopl::Value, eopl::ValueList;

  std::cout << "sizeof(eopl::Value): " << sizeof(eopl::Value) << std::endl;

  Value v = ValueList{{false, 1, 2.0, String("three"s), Symbol("four"s), ValueList{}}};

  std::cout << v.index() << std::endl;
  if (v.index() != 5) return 0;

  const ValueList& vlist = std::get<5>(v).get();
  for (const auto& x : vlist) {
    std::cout << x.index() << std::endl;
  }
}