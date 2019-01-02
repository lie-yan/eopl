//
// Created by robin on 2019-01-01.
//

#pragma once

#include <memory>
#include <utility>
#include "value.h"

namespace eopl {

class Env {
public:
  using SpEnv = std::shared_ptr<Env>;

  struct SymbolNotFoundError : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  Env () = default;
  Env (const Env&) = delete;
  Env& operator = (const Env&) = delete;
  Env (Env&&) = delete;
  Env& operator = (Env&&) = delete;

  Env (SpEnv parent, std::pair<Symbol, Value> pair);

  static SpEnv make_empty ();
  static SpEnv extend (SpEnv parent, Symbol sym, Value value);
  static const Value& apply (SpEnv env, const Symbol& sym);

private:
  SpEnv parent_;
  std::pair<Symbol, Value> pair_;
};

}
