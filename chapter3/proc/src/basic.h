//
// Created by robin on 2018-12-30.
//

#pragma once

namespace eopl {

/**
 * @brief Apply function `f` on the elements of range [first, last)
 *        successively. Between consecutive calls of `f`, function `g`
 *        is called.
 * @param first the start of range
 * @param last the end of range
 * @param f a unary function
 * @param g a function that accepts no argument
 */
template<typename InputIt, typename F, typename G>
void interleave (InputIt first, InputIt last, F f, G g) {
  
  if (first == last) {
    return;
  } else {
    f(*first++);
    while (first != last) {
      g();
      f(*first++);
    }
  }
}

}