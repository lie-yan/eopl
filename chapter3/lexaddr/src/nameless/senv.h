//
// Created by Lie Yan on 2019-01-27.
//

#pragma once

#include <ostream>
#include "value_fwd.h"

namespace eopl {

using SpStaticEnv = std::shared_ptr<struct StaticEnv>;

struct LexicalAddr {
  size_t senv_index = 0;
  size_t entry_index = 0;
  friend std::ostream& operator << (std::ostream& os, const LexicalAddr& addr);
};

class StaticEnv {
public:
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
  static LexicalAddr apply (const SpStaticEnv& env, const Symbol& sym);

private:
  SpStaticEnv parent_;
  std::vector<Symbol> vars_;
};


}