//
// Created by Lie Yan on 2019-01-26.
//

#include "env.h"
#include "value.h"
#include "exception.h"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <numeric>
#include <gsl/gsl>

namespace eopl {

Ribcage::Ribcage (std::vector<Symbol> variables, std::vector<Value> values)
    : variables_(std::move(variables)), values_(std::move(values)) {

  Expects(this->variables_.size() == this->values_.size());
}

std::optional<std::pair<Value, long>> Ribcage::find (const Symbol& var) const {

  auto found = std::find(std::begin(variables_), std::end(variables_), var);
  if (found == std::end(variables_)) {
    return {};
  } else {
    auto index = std::distance(std::begin(variables_), found);
    return {{values_[index], index}};
  }
}

Value Ribcage::operator [] (int index) const {
  return values_[index];
}

Env::Env (SpEnv parent, Symbol sym, Value val)
    : parent_(std::move(parent)), ribcage_{{std::move(sym)},
                                           {std::move(val)}} { }

Env::Env (SpEnv parent, std::vector<Symbol> syms, std::vector<Value> values)
    : parent_(std::move(parent)), ribcage_{std::move(syms), std::move(values)} { }

SpEnv Env::make_empty () { return SpEnv(); }

SpEnv Env::extend (SpEnv parent, Symbol sym, Value value) {
  return std::make_shared<Env>(std::move(parent), std::move(sym), std::move(value));
}

SpEnv Env::extend (SpEnv parent, std::vector<Symbol> syms, std::vector<Value> values) {
  return std::make_shared<Env>(std::move(parent), std::move(syms), std::move(values));
}

Value Env::apply (SpEnv env, const Symbol& sym) {

  while (env) {
    auto found = env->ribcage_.find(sym);
    if (found) return found->first;
    env = env->parent_;
  }

  throw SymbolNotFoundError(fmt::format("Symbol {} not found.", sym));
}


}