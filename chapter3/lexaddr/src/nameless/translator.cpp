//
// Created by Lie Yan on 2019-01-27.
//

#include "translator.h"

namespace eopl {

static
SpStaticEnv make_initial_senv () {
  auto ret = StaticEnv::make_empty();
  ret = StaticEnv::extend(ret, Symbol{"emptylist"});
  return ret;
}

Program translation_of (const Program& program) {
  return Program{translation_of(program.exp1, make_initial_senv())};
}

struct TranslationOfVisitor {
  SpStaticEnv& env;

  template<typename T>
  Expression operator () (const boost::recursive_wrapper<T>& e) { return translation_of(e.get(), env); }

  template<typename T>
  Expression operator () (const T& e) { return translation_of(e, env); }

};

Expression translation_of (const Expression& exp, SpStaticEnv env) {
  return std::visit(TranslationOfVisitor{env}, *exp);
}

Expression translation_of (const ConstExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const VarExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const NamelessVarExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const IfExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const LetExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const NamelessLetExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const CondExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const UnpackExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const ProcExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const NamelessProcExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const CallExp& exp, SpStaticEnv env) {
  return Expression();
}

Expression translation_of (const LetrecExp& exp, SpStaticEnv env) {
  return Expression();
}

std::vector<Expression> translation_of (const std::vector<Expression>& exps, SpStaticEnv env) {
  return std::vector<Expression>();
}
}