#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <numeric>

int main () {
  std::vector v = {1, 2, 3, 4, 5};
  auto prod = std::accumulate(std::begin(v),
                              std::end(v),
                              1,
                              std::multiplies());
  std::cout << prod << std::endl;
}
