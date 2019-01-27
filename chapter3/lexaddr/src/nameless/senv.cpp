//
// Created by Lie Yan on 2019-01-27.
//

#include "senv.h"

#include "exception.h"
#include <fmt/ostream.h>

namespace eopl {

StaticEnv::StaticEnv (SpStaticEnv parent, Symbol sym)
    : parent_(std::move(parent)), vars_({std::move(sym)}) { }

StaticEnv::StaticEnv (SpStaticEnv parent, std::vector<Symbol> syms)
    : parent_(std::move(parent)), vars_(std::move(syms)) { }

SpStaticEnv StaticEnv::make_empty () {
  return SpStaticEnv();
}

SpStaticEnv StaticEnv::extend (SpStaticEnv parent, Symbol sym) {
  return std::make_shared<StaticEnv>(std::move(parent), std::move(sym));
}

SpStaticEnv StaticEnv::extend (SpStaticEnv parent, std::vector<Symbol> syms) {
  return std::make_shared<StaticEnv>(std::move(parent), std::move(syms));
}

/**
 * @post (sym ∉ env ∧ throw) ∨ (sym ∈ env ∧ return)
 */
LexicalAddr StaticEnv::apply (const SpStaticEnv& env, const Symbol& sym) {

  auto error_message = [&sym] () { return fmt::format("Symbol {} not found", sym); };

  if (!env) throw SymbolNotFoundError(error_message());

  int senv_index = 0;
  auto p = env;

  do {
    auto it = std::find(std::begin(p->vars_), std::end(p->vars_), sym);
    if (it != std::end(p->vars_)) {
      auto entry_index = std::distance(std::begin(p->vars_), it);
      return {(size_t)senv_index, (size_t)entry_index};
    } else {
      senv_index++;
      p = p->parent_;
    }
  } while (p);

  throw SymbolNotFoundError(error_message());
}

std::ostream& operator << (std::ostream& os, const LexicalAddr& addr) {
  os << "LexicalAddr(senv_index: " << addr.senv_index
     << ", entry_index: " << addr.entry_index << ")";
  return os;
}
}