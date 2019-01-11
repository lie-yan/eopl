//
// Created by robin on 2019-01-10.
//

#include "interpreter.h"

namespace eopl {

Value value_of (const Program& program, SpEnv env) {
  return value_of(program.exp1, std::move(env));
}

Value value_of (const Expression& expression, SpEnv env) {
  struct EvalVisitor {
    const SpEnv& env;
    Value operator () (const ConstExp& exp) { return value_of(exp, env); }
    Value operator () (const VarExp& exp) { return value_of(exp, env); }
    Value operator () (const RwDiffExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwMinusExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwZeroTestExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwIfExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwLetExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const DiffExp& exp) { return value_of(exp, env); }
    Value operator () (const MinusExp& exp) { return value_of(exp, env); }
    Value operator () (const ZeroTestExp& exp) { return value_of(exp, env); }
    Value operator () (const IfExp& exp) { return value_of(exp, env); }
    Value operator () (const LetExp& exp) { return value_of(exp, env); }
  };
  return std::visit(EvalVisitor{env}, expression);
}

Value value_of (const ConstExp& exp, SpEnv env) {
  return exp.num;
}
Value value_of (const VarExp& exp, SpEnv env) {
  return Env::apply(std::move(env), exp.var);
}
Value value_of (const DiffExp& exp, SpEnv env) {
  Value val1 = value_of(exp.exp1, env);
  Value val2 = value_of(exp.exp2, env);
  int i1 = value_to_int(val1).val;
  int i2 = value_to_int(val2).val;
  return Int{i1 - i2};
}
Value value_of (const ZeroTestExp& exp, SpEnv env) {
  Value val = value_of(exp.exp1, std::move(env));
  int i = value_to_int(val).val;
  return Bool{i == 0};
}
Value value_of (const IfExp& exp, SpEnv env) {
  Value val1 = value_of(exp.exp1, env);
  bool b1 = value_to_bool(val1).val;
  if (b1) return value_of(exp.exp2, std::move(env));
  else return value_of(exp.exp3, std::move(env));
}
Value value_of (const LetExp& exp, SpEnv env) {
  Value val1 = value_of(exp.exp1, env);
  auto new_env = Env::extend(std::move(env), exp.var, std::move(val1));
  return value_of(exp.body, std::move(new_env));
}
Value value_of (const MinusExp& exp, SpEnv env) {
  Value val1 = value_of(exp.exp1, std::move(env));
  int i = value_to_int(val1).val;
  return Int{-i};
}
}