//
// Created by robin on 2018-12-30.
//

#pragma once

namespace eopl {

template<typename InputIt, typename F, typename G>
void interleave (InputIt first, InputIt last, F f, G g) {
  if (first == last) return;

  f(*first++);
  while (first != last) {
    g();
    f(*first++);
  }
}

}