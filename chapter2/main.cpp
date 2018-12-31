#include <iostream>
#include "value.h"

int main () {
  using namespace std::string_literals;
  using namespace eopl;

  std::cout << "sizeof(Value): " << sizeof(Value) << std::endl;

  Value v = ValueArray{
      {},
      true,
      2,
      3.0,
      String("vier"),
      Symbol("fünf"),
      Pair{6, Pair{Symbol("sechs"), Pair{String("six"), {}}}},
      ValueArray{7, String("huit"), Symbol("neuf"), Pair{10, 11.11}}
  };

  std::cout << v.index() << std::endl;

  const ValueArray& vlist = std::get<RwValueArray>(v).get();

  std::for_each(std::begin(vlist), std::end(vlist),
                [] (const Value& x) {
                  std::cout << x.index() << ": " << x << std::endl;
                });

  std::cout << v << std::endl;

}