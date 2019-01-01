#include <iostream>
#include "value.h"
#include "environment.h"

int main () {
  using namespace eopl;

  Environment env;
  std::cout << "sizeof(Environment): " << sizeof(Environment) << std::endl;
}