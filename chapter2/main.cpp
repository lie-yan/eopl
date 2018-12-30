#include <iostream>
#include "scope_support.h"

int main () {
  using namespace std::string_literals;
  using namespace eopl;

  std::cout << "sizeof(Value): " << sizeof(Value) << std::endl;

  Value v = ValueList{
      {},
      false,
      1,
      2.0,
      String("three"),
      Symbol("four"),
      ValueList{5, 6, 7}
  };

  std::cout << v.index() << std::endl;

  const ValueList& vlist = std::get<RwValueList>(v).get();

  std::for_each(std::begin(vlist), std::end(vlist),
                [] (const Value& x) {
                  std::cout << x << std::endl;
                });

}