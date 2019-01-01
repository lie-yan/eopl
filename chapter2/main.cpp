#include <iostream>
#include "value.h"

int main () {
  using namespace std::string_literals;
  using namespace eopl;

  std::cout << "sizeof(Value): " << sizeof(Value) << std::endl;

  Value value = Array{
      {},
      true,
      2,
      3.0,
      String("vier"),
      Symbol("fünf"),
      Pair{6, Pair{Symbol("sechs"), Pair{String("six"), {}}}},
      Array{7, String("huit"), Symbol("neuf"), Pair{10, 11.11}}
  };

  std::cout << value.index() << std::endl;

  const Array& array = std::get<RwArray>(value).get();

  std::for_each(std::begin(array), std::end(array),
                [] (const auto& item) {
                  std::cout << item.index() << ": " << item << std::endl;
                });

  std::cout << value << std::endl;

}