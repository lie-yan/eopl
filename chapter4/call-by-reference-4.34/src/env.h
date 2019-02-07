//
// Created by robin on 2019-01-01.
//

#pragma once

#include <memory>
#include "value_fwd.h"

namespace eopl {

using SpEnv = std::shared_ptr<struct Env>;

struct Ribcage {

  Ribcage () = default;
  Ribcage (const Ribcage&) = default;
  Ribcage (Ribcage&&) = default;
  Ribcage& operator = (const Ribcage&) = default;
  Ribcage& operator = (Ribcage&&) = default;

  Ribcage (std::vector<Symbol> variables, std::vector<Ref> values);
  std::optional<std::pair<Ref, long>> find (const Symbol& var) const;
  Ref operator [] (int index) const;

private:
  std::vector<Symbol> variables_;
  std::vector<Ref> values_;
};

class Env {
public:

  Env () = default;
  Env (const Env&) = delete;
  Env& operator = (const Env&) = delete;
  Env (Env&&) = delete;
  Env& operator = (Env&&) = delete;

  Env (SpEnv parent, Symbol sym, Ref val);
  Env (SpEnv parent, std::vector<Symbol> syms, std::vector<Ref> values);

  static SpEnv make_empty ();
  static SpEnv extend (SpEnv parent, Symbol sym, Ref value);
  static SpEnv extend (SpEnv parent, std::vector<Symbol> syms, std::vector<Ref> values);
  static Ref apply (SpEnv env, const Symbol& sym);

private:
  SpEnv parent_;
  Ribcage ribcage_;
};

}
