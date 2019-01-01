//
// Created by robin on 2019-01-01.
//

#include "environment.h"
#include <fmt/format.h>

namespace eopl {

Environment::Environment (Environment::SpEnv parent, std::pair<Symbol, Value> pair)
    : parent_(std::move(parent)), pair_(std::move(pair)) { }

Environment::SpEnv Environment::make_empty () { return Environment::SpEnv(); }

Environment::SpEnv Environment::extend (Environment::SpEnv parent, Symbol sym, Value value) {
  return std::make_shared<Environment>(std::move(parent),
                                       std::pair(std::move(sym), std::move(value)));
}

const Value& Environment::apply (Environment::SpEnv env, const Symbol& sym) {
  if (!env) throw SymbolNotFoundError(fmt::format("Symbol {} not found.", sym));
  else if (env->pair_.first == sym) return env->pair_.second;
  else return apply(env->parent_, sym);
}

}