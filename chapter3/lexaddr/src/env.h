//
// Created by robin on 2019-01-01.
//

#pragma once

#include <memory>
#include <utility>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <numeric>

namespace eopl {

struct SymbolNotFoundError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

template<typename Symbol, typename Value, typename Expression>
//   requires Regular<Symbol>, Semiregular<Value>
class Environment {
public:

  struct SvPair {
    Symbol symbol;
    Value value;
  };

  using SpEnv = std::shared_ptr<Environment>;

  Environment () = default;
  Environment (const Environment&) = delete;
  Environment& operator = (const Environment&) = delete;
  Environment (Environment&&) = delete;
  Environment& operator = (Environment&&) = delete;

  Environment (SpEnv parent, SvPair pair)
      : parent_(std::move(parent)), bound_record_(std::move(pair)) { }

  static SpEnv make_empty () { return SpEnv(); }

  static SpEnv extend (SpEnv parent, Symbol sym, Value value) {
    return std::make_shared<Environment>(std::move(parent),
                                         SvPair{std::move(sym), std::move(value)});
  }

  static SpEnv extend (SpEnv parent,
                       std::vector<Symbol> syms,
                       std::vector<Value> values) {
    assert(syms.size() == values.size());
    return std::inner_product(std::begin(syms),
                              std::end(syms),
                              std::begin(values),
                              parent,
                              [] (auto&& acc, auto&& pair) {
                                return extend(std::forward<decltype(acc)>(acc),
                                              std::move(pair.symbol),
                                              std::move(pair.value));
                              },
                              [] (auto&& sym, auto&& val) {
                                return SvPair{std::forward<decltype(sym)>(sym),
                                              std::forward<decltype(val)>(val)};
                              });
  }

  static Value apply (Environment::SpEnv env, const Symbol& sym) {

    for (auto p = env; p; p = p->parent_) {
      if (p->bound_record_.symbol == sym) {
        return p->bound_record_.value;
      }
    }
    throw SymbolNotFoundError(fmt::format("Symbol {} not found.", sym));
  }

private:
  SpEnv parent_;
  SvPair bound_record_;
};

}
