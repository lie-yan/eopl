//
// Created by robin on 2019-01-01.
//

#pragma once

#include <memory>
#include "value_fwd.h"

namespace eopl {

using SpEnv = std::shared_ptr<struct Env>;
using WpEnv = std::weak_ptr<struct Env>;

struct Ribcage {

  Ribcage () = default;
  Ribcage (const Ribcage&) = default;
  Ribcage (Ribcage&&) = default;
  Ribcage& operator = (const Ribcage&) = default;
  Ribcage& operator = (Ribcage&&) = default;

  Ribcage (std::vector<Symbol> vars, std::vector<Value> values);
  std::optional<std::pair<Value, int>> find (const Symbol& var) const;
  Value operator [] (int index) const;

private:
  std::vector<Symbol> vars;
  std::vector<Value> values;
};

class Env {
public:

  Env () = default;
  Env (const Env&) = delete;
  Env& operator = (const Env&) = delete;
  Env (Env&&) = delete;
  Env& operator = (Env&&) = delete;

  Env (SpEnv parent, Symbol sym, Value val);
  Env (SpEnv parent, std::vector<Symbol> syms, std::vector<Value> values);

  static SpEnv make_empty ();
  static SpEnv extend (SpEnv parent, Symbol sym, Value value);
  static SpEnv extend (SpEnv parent, std::vector<Symbol> syms, std::vector<Value> values);
  static Value apply (SpEnv env, const Symbol& sym);

private:
  SpEnv parent_;
  Ribcage ribcage_;
};

}
