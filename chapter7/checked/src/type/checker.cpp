//
// Created by Lie Yan on 2019-02-10.
//

#include "checker.h"

#include "type.h"
#include "expr.h"
#include <gsl/gsl>

namespace eopl::type {

Type type_of (const Program& program, const SpTEnv& tenv) {
  return type_of(program.exp1, make_initial_tenv());
}

struct TypeCheckVisitor {
  const SpTEnv& tenv;

  template<typename T>
  Type operator () (const boost::recursive_wrapper<T>& t) { return type_of(t.get(), tenv); }

  template<typename T>
  Type operator () (const T& t) { return type_of(t, tenv); }
};

Type type_of (const Expression& expression, const SpTEnv& tenv) {
  return std::visit(TypeCheckVisitor{tenv}, *expression);
}

Type type_of (const ConstExp& exp, const SpTEnv& tenv) {
  return to_type(IntType{});
}

Type type_of (const VarExp& exp, const SpTEnv& tenv) {
  return TEnv::apply(tenv, exp.var);
}

Type type_of (const IfExp& exp, const SpTEnv& tenv) {
  Type cond_type = type_of(exp.cond, tenv);
  check_equal_type(cond_type, to_type(BoolType{}), exp.cond);
  Type then_type = type_of(exp.then_, tenv);
  Type else_type = type_of(exp.else_, tenv);
  check_equal_type(else_type, then_type, exp.else_);
  return then_type;
}

Type type_of (const LetExp& exp, const SpTEnv& tenv, std::true_type) {
  Expects(exp.star);
  auto new_tenv = std::accumulate(std::begin(exp.clauses),
                                  std::end(exp.clauses),
                                  tenv,
                                  [] (const SpTEnv& acc, const LetExp::Clause& c) {
                                    auto var_type = type_of(c.second, acc);
                                    return TEnv::extend(acc, c.first, var_type);
                                  });
  return type_of(exp.body, new_tenv);
}

Type type_of (const LetExp& exp, const SpTEnv& tenv, std::false_type) {
  Expects(!exp.star);

  std::vector<Type> var_types;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(var_types),
                 [&tenv] (const LetExp::Clause& c) {
                   return type_of(c.second, tenv);
                 });
  std::vector<Symbol> symbols;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(symbols),
                 [] (const LetExp::Clause& c) { return c.first; });
  SpTEnv new_env = TEnv::extend(tenv, symbols, var_types);
  Type result_type = type_of(exp.body, new_env);
  return result_type;
}

Type type_of (const LetExp& exp, const SpTEnv& tenv) {
  if (exp.star) return type_of(exp, tenv, std::true_type());
  else return type_of(exp, tenv, std::false_type());
}

Type type_of (const CondExp& exp, const SpTEnv& tenv) {
  return Type();
}

Type type_of (const UnpackExp& exp, const SpTEnv& tenv) {
  return Type();
}

Type type_of (const ProcExp& exp, const SpTEnv& tenv) {
  return Type();
}

Type type_of (const CallExp& exp, const SpTEnv& tenv) {
  return Type();
}

Type type_of (const LetrecExp& exp, const SpTEnv& tenv) {
  return Type();
}

SpTEnv make_initial_tenv () {
  return SpTEnv();
}

void check_program (const std::string& text) {

}

void check_equal_type (const Type& actual, const Type& expected, const Expression& expression) {

}


}