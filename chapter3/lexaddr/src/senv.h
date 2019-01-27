//
// Created by Lie Yan on 2019-01-27.
//

#pragma once

#include "value_fwd.h"

namespace eopl {

using SpStaticEnv = std::shared_ptr<struct StaticEnv>;

struct LexicalAddr {
  int senv_index;
  long entry_index;
};

struct StaticEnv {

  StaticEnv () = default;
  StaticEnv (const StaticEnv&) = delete;
  StaticEnv& operator = (const StaticEnv&) = delete;
  StaticEnv (StaticEnv&&) = delete;
  StaticEnv& operator = (StaticEnv&&) = delete;

  StaticEnv (SpStaticEnv parent, Symbol sym);
  StaticEnv (SpStaticEnv parent, std::vector<Symbol> syms);

  static SpStaticEnv make_empty ();
  static SpStaticEnv extend (SpStaticEnv parent, Symbol sym);
  static SpStaticEnv extend (SpStaticEnv parent, std::vector<Symbol> syms);
  static LexicalAddr apply (SpStaticEnv env, const Symbol& sym);

private:
  SpStaticEnv parent_;
  std::vector<Symbol> vars_;
};


}