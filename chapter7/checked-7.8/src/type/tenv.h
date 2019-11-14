//
// Created by Lie Yan on 2019-02-10.
//

#pragma once

#include <memory>

#include "value_fwd.h"
#include "type_fwd.h"

namespace eopl {
struct ParamDecl;
}

namespace eopl::type {

using SpTEnv = std::shared_ptr<struct TEnv>;

struct Ribcage {

  Ribcage () = default;
  Ribcage (const Ribcage&) = default;
  Ribcage (Ribcage&&) = default;
  Ribcage& operator = (const Ribcage&) = default;
  Ribcage& operator = (Ribcage&&) = default;

  Ribcage (std::vector<Symbol> variables, std::vector<Type> values);
  std::optional<std::pair<Type, long>> find (const Symbol& var) const;
  Type operator [] (int index) const;

private:
  std::vector<Symbol> variables_;
  std::vector<Type> values_;
};

class TEnv {
public:

  TEnv () = default;
  TEnv (const TEnv&) = delete;
  TEnv& operator = (const TEnv&) = delete;
  TEnv (TEnv&&) = delete;
  TEnv& operator = (TEnv&&) = delete;

  TEnv (SpTEnv parent, Symbol sym, Type ty);
  TEnv (SpTEnv parent, std::vector<Symbol> syms, std::vector<Type> ty_list);

  static SpTEnv make_empty ();
  static SpTEnv extend (SpTEnv parent, Symbol sym, Type ty);
  static SpTEnv extend (SpTEnv parent, std::vector<Symbol> syms, std::vector<Type> ty_list);

  static SpTEnv extend (SpTEnv parent, ParamDecl param, Type ty);
  static SpTEnv extend (SpTEnv parent, std::vector<ParamDecl> params, std::vector<Type> ty_list);

  static Type apply (SpTEnv env, const Symbol& sym);

private:
  SpTEnv parent_;
  Ribcage ribcage_;
};

}