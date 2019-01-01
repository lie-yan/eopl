#include <iostream>
#include "environment.h"

int main () {
  using namespace eopl;

  std::cout << "sizeof(Environment): " << sizeof(Environment) << std::endl;

  Environment::SpEnv env = Environment::make_empty();

  auto x = Symbol("x");
  auto env1 = Environment::extend(env, x, 1);
  std::cout << x << ": " << Environment::apply(env1, x) << std::endl;
  auto y = Symbol("y");

  {
    auto env2 = Environment::extend(env1, y, String("hello"));
    auto env3 = Environment::extend(env2, x, 1.2);
    std::cout << x << ": " << Environment::apply(env3, x) << std::endl;
  }

  try {
    Environment::apply(env, y);
  } catch (const Environment::SymbolNotFoundError& e) {
    std::cout << e.what() << std::endl;
  }
}