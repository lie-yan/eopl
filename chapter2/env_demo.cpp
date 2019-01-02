#include <iostream>
#include "env.h"

int main () {
  using namespace eopl;

  std::cout << "sizeof(Env): " << sizeof(Env) << std::endl;

  Env::SpEnv env0 = Env::make_empty();

  auto x = Symbol("x");
  auto env1 = Env::extend(env0, x, 1);
  std::cout << x << ": " << Env::apply(env1, x) << std::endl;
  auto y = Symbol("y");

  {
    auto env2 = Env::extend(env1, y, String("hello"));
    auto env3 = Env::extend(env2, x, 1.2);
    std::cout << x << ": " << Env::apply(env3, x) << std::endl;
  }

  try {
    Env::apply(env1, y);
  } catch (const Env::SymbolNotFoundError& e) {
    std::cout << e.what() << std::endl;
  }
}