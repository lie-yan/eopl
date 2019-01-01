//
// Created by robin on 2019-01-01.
//

#include "env.h"
#include <fmt/format.h>

namespace eopl {

Env::Env (Env::SpEnv parent, std::pair<Symbol, Value> pair)
    : parent_(std::move(parent)), pair_(std::move(pair)) { }

Env::SpEnv Env::make_empty () { return Env::SpEnv(); }

Env::SpEnv Env::extend (Env::SpEnv parent, Symbol sym, Value value) {
  return std::make_shared<Env>(std::move(parent),
                                       std::pair(std::move(sym), std::move(value)));
}

const Value& Env::apply (Env::SpEnv env, const Symbol& sym) {
  if (!env) throw SymbolNotFoundError(fmt::format("Symbol {} not found.", sym));
  else if (env->pair_.first == sym) return env->pair_.second;
  else return apply(env->parent_, sym);
}

}