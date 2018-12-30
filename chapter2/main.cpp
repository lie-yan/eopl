#include <iostream>
#include "scope_support.h"

int main () {
  using namespace std::string_literals;
  using eopl::Symbol, eopl::String, eopl::Value, eopl::ValueList;
  using eopl::RwValueList;

  std::cout << "sizeof(eopl::Value): " << sizeof(eopl::Value) << std::endl;

  Value v = ValueList{false, 1, 2.0, String("three"s), Symbol("four"s), ValueList{}};

  std::cout << v.index() << std::endl;
  
  const ValueList& vlist = std::get<RwValueList>(v).get();
  for (const auto& x : vlist) {
    std::cout << x.index() << std::endl;
  }
}