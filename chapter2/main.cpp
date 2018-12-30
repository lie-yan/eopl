#include <iostream>
#include "scope_support.h"

int main () {
  using namespace std::string_literals;
  using eopl::Symbol, eopl::String, eopl::Value, eopl::ValueList;

  std::cout << "sizeof(eopl::Value): " << sizeof(eopl::Value) << std::endl;

  Value v = ValueList{{false, 1, 2.0, String("three"s), Symbol("four"s), ValueList{}}};

  std::cout << v.index() << std::endl;
  const int index_vlist = 5;
  if (v.index() != index_vlist) return 0;

  const ValueList& vlist = std::get<index_vlist>(v).get();
  for (const auto& x : vlist) {
    std::cout << x.index() << std::endl;
  }
}