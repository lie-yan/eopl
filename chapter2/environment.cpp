//
// Created by robin on 2019-01-01.
//

#include "environment.h"

namespace eopl {

Environment::Environment (Environment::SpEnv parent, std::pair<Symbol, Value> pair)
    : parent_(std::move(parent)), pair_(std::move(pair)) { }

Environment::SpEnv Environment::make_empty () { return std::make_shared<Environment>(); }

const Value& Environment::apply (const Symbol& sym) {
  if (pair_.first == sym) return pair_.second;
  else if (!parent_) return parent_->apply(sym);
  else throw SymbolNotFoundError("Symbol not found");
}

Environment::SpEnv Environment::extend (Symbol sym, Value value) {
  return std::make_shared<Environment>(shared_from_this(),
                                       std::pair(std::move(sym), std::move(value)));
}

}