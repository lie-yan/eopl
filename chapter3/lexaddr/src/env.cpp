#include <utility>

//
// Created by Lie Yan on 2019-01-26.
//

#include "env.h"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <numeric>

namespace eopl {

Env::Env (SpEnv parent, SvPair pair)
    : parent_(std::move(parent)), bound_record_(std::move(pair)) { }

SpEnv Env::make_empty () { return SpEnv(); }

SpEnv Env::extend (SpEnv parent, Symbol sym, Value value) {
  return std::make_shared<Env>(std::move(parent),
                               SvPair{std::move(sym), std::move(value)});
}

SpEnv Env::extend (SpEnv parent, std::vector<Symbol> syms, std::vector<Value> values) {
  assert(syms.size() == values.size());
  return std::inner_product(std::begin(syms),
                            std::end(syms),
                            std::begin(values),
                            std::move(parent),
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

Value Env::apply (SpEnv env, const Symbol& sym) {

  for (auto p = env; p; p = p->parent_) {
    if (p->bound_record_.symbol == sym) {
      return p->bound_record_.value;
    }
  }
  throw SymbolNotFoundError(fmt::format("Symbol {} not found.", sym));
}


}