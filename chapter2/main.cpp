#include <iostream>
#include "value.h"
#include "environment.h"

int main () {
  using namespace eopl;

  std::cout << "sizeof(Environment): " << sizeof(Environment) << std::endl;

  Environment::SpEnv env = Environment::make_empty();

  auto x = Symbol("x");
  auto env1 = env->extend(x, 1);
  std::cout << x << ": " << env1->apply(x) << std::endl;

  try {
    env1->apply(Symbol("y"));
  } catch (const Environment::SymbolNotFoundError& e) {
    std::cout << e.what() << std::endl;
  }
}