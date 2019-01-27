//
// Created by robin on 2019-01-01.
//

#pragma once

#include <memory>
#include "value_fwd.h"

namespace eopl {

struct SymbolNotFoundError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using SpEnv = std::shared_ptr<struct Env>;

struct SvPair {
  Symbol symbol;
  Value value;
};

class Env {
public:

  Env () = default;
  Env (const Env&) = delete;
  Env& operator = (const Env&) = delete;
  Env (Env&&) = delete;
  Env& operator = (Env&&) = delete;

  Env (SpEnv parent, SvPair pair);

  static SpEnv make_empty ();
  static SpEnv extend (SpEnv parent, Symbol sym, Value value);
  static SpEnv extend (SpEnv parent, std::vector<Symbol> syms, std::vector<Value> values);

  static Value apply (SpEnv env, const Symbol& sym);

private:
  SpEnv parent_;
  SvPair bound_record_;
};

}
