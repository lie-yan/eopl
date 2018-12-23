#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <numeric>

int main () {
  std::vector<int> v = {1, 2, 3, 4};
  auto sum = std::accumulate(std::begin(v),
                             std::end(v),
                             1,
                             std::multiplies<>());
  std::cout << sum << std::endl;
  return 0;
}
