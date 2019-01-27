#include <utility>

//
// Created by Lie Yan on 2019-01-26.
//

#include "env.h"
#include "exception.h"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <numeric>

namespace eopl {

Ribcage::Ribcage (std::vector<Symbol> vars, std::vector<Value> values)
    : vars(std::move(vars)), values(std::move(values)) {

  assert(this->vars.size() == this->values.size());
}

std::optional<std::pair<Value, int>> Ribcage::find (const Symbol& var) const {

  auto it = std::begin(values);
  std::find_if(std::begin(vars),
               std::end(vars),
               [&it, &var] (const Symbol& symbol) {
                 if (symbol == var) {
                   return true;
                 } else {
                   it++;
                   return false;
                 }
               });
  if (it == std::end(values)) {
    return {};
  } else {
    return {{*it, std::distance(std::begin(values), it)}};
  }
}

Value Ribcage::operator [] (int index) const {
  return values[index];
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

  for (auto p = env; p; p = p->parent_) {
    auto found = p->ribcage_.find(sym);
    if (found) return found->first;
  }
  throw SymbolNotFoundError(fmt::format("Symbol {} not found.", sym));
}


}