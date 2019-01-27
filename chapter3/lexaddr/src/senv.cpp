//
// Created by Lie Yan on 2019-01-27.
//

#include "senv.h"

#include "exception.h"
#include <fmt/ostream.h>

namespace eopl {

StaticEnv::StaticEnv (SpStaticEnv parent, Symbol sym) : parent_(std::move(parent)), vars_({std::move(sym)}) { }

StaticEnv::StaticEnv (SpStaticEnv parent, std::vector<Symbol> syms) : parent_(std::move(parent)),
                                                                      vars_(std::move(syms)) { }

SpStaticEnv StaticEnv::make_empty () {
  return SpStaticEnv();
}

SpStaticEnv StaticEnv::extend (SpStaticEnv parent, Symbol sym) {
  return std::make_shared<StaticEnv>(std::move(parent), std::move(sym));
}

SpStaticEnv StaticEnv::extend (SpStaticEnv parent, std::vector<Symbol> syms) {
  return std::make_shared<StaticEnv>(std::move(parent), std::move(syms));
}

LexicalAddr StaticEnv::apply (SpStaticEnv env, const Symbol& sym) {
  LexicalAddr ret = {0, 0};
  for (auto p = env; p; p = p->parent_) {
    auto it = std::find(std::begin(p->vars_),
                        std::end(p->vars_),
                        sym);
    if (it != std::end(p->vars_)) {
      ret.entry_index = std::distance(std::begin(p->vars_), it);
      return ret;
    } else {
      ret.senv_index++;
    }
  }

  throw SymbolNotFoundError(fmt::format("Symbol {} not found", sym));
}


}