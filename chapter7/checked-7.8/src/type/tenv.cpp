//
// Created by Lie Yan on 2019-02-10.
//

#include "tenv.h"
#include "expr.h"
#include "exception.h"
#include <fmt/ostream.h>
#include <gsl/gsl>

namespace eopl::type {

Ribcage::Ribcage (std::vector<Symbol> variables, std::vector<Type> values)
    : variables_(std::move(variables)), values_(std::move(values)) {

  Expects(this->variables_.size() == this->values_.size());
}

std::optional<std::pair<Type, long>> Ribcage::find (const Symbol& var) const {

  auto found = std::find(std::begin(variables_), std::end(variables_), var);
  if (found == std::end(variables_)) {
    return {};
  } else {
    auto index = std::distance(std::begin(variables_), found);
    return {{values_[index], index}};
  }
}

Type Ribcage::operator [] (int index) const {
  return values_[index];
}

TEnv::TEnv (SpTEnv parent, Symbol sym, Type ty)
    : parent_(std::move(parent)), ribcage_{{std::move(sym)},
                                           {std::move(ty)}} { }

TEnv::TEnv (SpTEnv parent, std::vector<Symbol> syms, std::vector<Type> ty_list)
    : parent_(std::move(parent)), ribcage_{std::move(syms), std::move(ty_list)} { }

SpTEnv TEnv::make_empty () { return SpTEnv(); }

SpTEnv TEnv::extend (SpTEnv parent, Symbol sym, Type ty) {
  return std::make_shared<TEnv>(std::move(parent), std::move(sym), std::move(ty));
}

SpTEnv TEnv::extend (SpTEnv parent, std::vector<Symbol> syms, std::vector<Type> ty_list) {
  return std::make_shared<TEnv>(std::move(parent), std::move(syms), std::move(ty_list));
}

SpTEnv TEnv::extend (SpTEnv parent, ParamDecl param, Type ty) {
  return std::make_shared<TEnv>(std::move(parent), std::move(param.sym), std::move(ty));
}

SpTEnv TEnv::extend (SpTEnv parent, std::vector<ParamDecl> params, std::vector<Type> ty_list) {
  std::vector<Symbol> syms;
  std::transform(std::make_move_iterator(std::begin(params)),
                 std::make_move_iterator(std::end(params)),
                 std::back_inserter(syms),
                 [] (ParamDecl&& paramDecl) { return std::move(paramDecl.sym); });
  return std::make_shared<TEnv>(std::move(parent), std::move(syms), std::move(ty_list));
}

Type TEnv::apply (SpTEnv env, const Symbol& sym) {

  while (env) {
    auto found = env->ribcage_.find(sym);
    if (found) return found->first;
    env = env->parent_;
  }

  throw SymbolNotFoundError(fmt::format("Symbol {} not found.", sym));
}

}