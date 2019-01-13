//
// Created by robin on 2019-01-10.
//

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <numeric>
#include "interpreter.h"
#include "builtin.h"
#include "lex.yy.h"

namespace eopl {

Value value_of (const Program& program, SpEnv env) {
  return value_of(program.exp1, std::move(env));
}

Value value_of (const Expression& exp, SpEnv env) {
  struct EvalVisitor {
    const SpEnv& env;
    Value operator () (const ConstExp& exp) { return value_of(exp, env); }
    Value operator () (const VarExp& exp) { return value_of(exp, env); }
    Value operator () (const RwIfExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwLetExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwOpExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwCondExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const IfExp& exp) { return value_of(exp, env); }
    Value operator () (const LetExp& exp) { return value_of(exp, env); }
    Value operator () (const OpExp& exp) { return value_of(exp, env); }
    Value operator () (const CondExp& exp) { return value_of(exp, env); }
  };
  return std::visit(EvalVisitor{env}, exp);
}

Value value_of (const ConstExp& exp, SpEnv env) {
  return int_to_value(exp.num);
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
  if (exp.starred) {
    auto new_env = std::accumulate(std::begin(exp.clauses),
                                   std::end(exp.clauses),
                                   env,
                                   [] (SpEnv acc, const LetExp::Clause& c) -> SpEnv {
                                     auto res = value_of(c.second, acc);
                                     return Env::extend(std::move(acc), c.first, std::move(res));
                                   });
    return value_of(exp.body, std::move(new_env));
  } else {
    auto new_env = std::accumulate(std::begin(exp.clauses),
                                   std::end(exp.clauses),
                                   env,
                                   [&env] (SpEnv acc, const LetExp::Clause& c) -> SpEnv {
                                     auto res = value_of(c.second, env);
                                     return Env::extend(std::move(acc), c.first, std::move(res));
                                   });
    return value_of(exp.body, std::move(new_env));
  }
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

Value value_of (const CondExp& exp, SpEnv env) {
  auto it = std::find_if(std::begin(exp.clauses),
                         std::end(exp.clauses),
                         [env] (const CondExp::Clause& c) -> bool {
                           auto b = value_of(c.first, env);
                           return value_to_bool(b).get();
                         });
  if (it == std::end(exp.clauses)) {
    throw std::runtime_error("at least one clause should be true for the "
                             "cond expression, but none were found");
  } else {
    return value_of(it->second, env);
  }
}

SpEnv make_initial_env () {
  auto ret = Env::make_empty();
  ret = Env::extend(ret, Symbol{"emptylist"}, nil_to_value(Nil()));
  return ret;
}

Value eval (const std::string& s) {
  std::istringstream ss(s);
  yy::Lexer lexer(ss);
  Program result;
  yy::parser p(lexer, result);
  p.set_debug_level(getenv("YYDEBUG") != nullptr);
  p.parse();

  return value_of(result, make_initial_env());
}

}