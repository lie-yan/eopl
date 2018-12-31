#include <iostream>
#include "value.h"

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
      Pair{5, Pair{Symbol("sechs"), Pair{6.5, {}}}},
      ValueList{String("sept"), 8, Symbol("neun"), Pair{10, 10.3}}
  };

  std::cout << v.index() << std::endl;

  const ValueList& vlist = std::get<RwValueList>(v).get();

  std::for_each(std::begin(vlist), std::end(vlist),
                [] (const Value& x) {
                  std::cout << x.index() << ": " << x << std::endl;
                });

}