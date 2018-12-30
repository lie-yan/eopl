#include <iostream>
#include "scope_support.h"
#include <string>

int main () {
  using namespace std::string_literals;

  std::cout << "sizeof(eopl::Value): " << sizeof(eopl::Value) << std::endl;

  eopl::Value v = eopl::ValueList{{false, 2, 3.0, "w"s}};
  std::cout << v.index() << std::endl;
  eopl::ValueList& vlist = std::get<4>(v).get();
  for (auto& x : vlist) {
    std::cout << x.index() << std::endl;
  }
}