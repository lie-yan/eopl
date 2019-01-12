//
// Created by robin on 2019-01-10.
//

#include "interpreter.h"
#include "builtin.h"

namespace eopl {

Value value_of (const Program& program, SpEnv env) {
  return value_of(program.exp1, std::move(env));
}

Value value_of (const Expression& expression, SpEnv env) {
  struct EvalVisitor {
    const SpEnv& env;
    Value operator () (const ConstExp& exp) { return value_of(exp, env); }
    Value operator () (const VarExp& exp) { return value_of(exp, env); }
    Value operator () (const RwIfExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwLetExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwOpExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const IfExp& exp) { return value_of(exp, env); }
    Value operator () (const LetExp& exp) { return value_of(exp, env); }
    Value operator () (const OpExp& exp) { return value_of(exp, env); }
  };
  return std::visit(EvalVisitor{env}, expression);
}

Value value_of (const ConstExp& exp, SpEnv env) {
  return exp.num;
}

Value value_of (const VarExp& exp, SpEnv env) {
  return Env::apply(std::move(env), exp.var);
}

Value value_of (const IfExp& exp, SpEnv env) {
  Value val1 = value_of(exp.exp1, env);
  bool b1 = value_to_bool(val1).get();
  if (b1) return value_of(exp.exp2, std::move(env));
  else return value_of(exp.exp3, std::move(env));
}

Value value_of (const LetExp& exp, SpEnv env) {
  Value val1 = value_of(exp.exp1, env);
  auto new_env = Env::extend(std::move(env), exp.var, std::move(val1));
  return value_of(exp.body, std::move(new_env));
}

Value value_of (const OpExp& exp, SpEnv env) {
  std::vector<Value> values;
  std::transform(std::begin(exp.rands),
                 std::end(exp.rands),
                 std::back_inserter(values),
                 [&env] (const auto& e) -> Value {
                   return value_of(e, env);
                 });
  auto fun = builtin::find_builtin(exp.rator);
  if (fun) {
    return (*fun)(values);
  } else {
    throw std::runtime_error(fmt::format("function {} does not exist", exp.rator));
  }
}

}