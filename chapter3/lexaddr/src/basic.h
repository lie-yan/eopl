//
// Created by robin on 2018-12-30.
//

#pragma once

namespace eopl {

/**
 * @brief Convenient visitor for <code>std::variant</code>.
 *  See also <a href="https://en.cppreference.com/w/cpp/utility/variant/visit">
 *  https://en.cppreference.com/w/cpp/utility/variant/visit</a>
 * @tparam Ts
 */
template<class... Ts>
struct overloaded : Ts ... { using Ts::operator ()...; };
template<class... Ts> overloaded (Ts...) -> overloaded<Ts...>;

/**
 * @brief Apply function <code>f</code> on the elements of range
 *        <code>[first, last)</code> successively. Between consecutive calls to
 *        <code>f</code>, invoke function <code>g</code>.
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