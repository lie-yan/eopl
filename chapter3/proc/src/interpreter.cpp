#include <utility>

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
    Value operator () (const RwUnpackExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwProcExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwCallExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const IfExp& exp) { return value_of(exp, env); }
    Value operator () (const LetExp& exp) { return value_of(exp, env); }
    Value operator () (const OpExp& exp) { return value_of(exp, env); }
    Value operator () (const CondExp& exp) { return value_of(exp, env); }
    Value operator () (const UnpackExp& exp) { return value_of(exp, env); }
    Value operator () (const ProcExp& exp) { return value_of(exp, env); }
    Value operator () (const CallExp& exp) { return value_of(exp, env); }
  };
  return std::visit(EvalVisitor{env}, *exp);
}

Value value_of (const ConstExp& exp, SpEnv env) {
  return int_to_value(exp.num);
}

Value value_of (const VarExp& exp, SpEnv env) {
  return Env::apply(std::move(env), exp.var);
}

Value value_of (const IfExp& exp, SpEnv env) {
  Value val1 = value_of(exp.cond, env);
  bool b1 = value_to_bool(val1).get();
  if (b1) return value_of(exp.then_, std::move(env));
  else return value_of(exp.else_, std::move(env));
}

Value value_of (const LetExp& exp, SpEnv env) {
  if (exp.star) {
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
  auto fun = built_in::find_built_in(exp.rator);
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

Value value_of (const UnpackExp& exp, SpEnv env) {
  static const std::string msg = "the size of identifier list and that of the pack "
                                 "does not match";

  auto lst = value_of(exp.pack, env);
  using P = decltype(std::pair(lst, env));
  P p = std::accumulate(std::begin(exp.vars),
                        std::end(exp.vars),
                        P(lst, env),
                        [] (P acc, const Symbol& s) -> P {
                          if (auto type = type_of(acc.first); type == ValueType::PAIR) {
                            auto& pair = value_to_pair(acc.first);
                            auto new_env = Env::extend(std::move(acc.second), s, pair.first);
                            return std::pair(pair.second, std::move(new_env));
                          } else {
                            throw std::runtime_error(msg);
                          }
                        });
  if (auto type = type_of(p.first); type != ValueType::NIL) {
    throw std::runtime_error(msg);
  } else {
    return value_of(exp.body, std::move(p.second));
  }
}

Value value_of (const ProcExp& exp, SpEnv env) {
  return proc_to_value(Proc{exp.vars, exp.body, std::move(env)});
}

std::vector<Value> value_of (const std::vector<Expression>& exps, SpEnv env) {
  std::vector<Value> results;
  std::transform(std::begin(exps),
                 std::end(exps),
                 std::back_inserter(results),
                 [&env] (const auto& e) -> Value {
                   return value_of(e, env);
                 });
  return results;
}

Value value_of (const CallExp& exp, SpEnv env) {

  auto eval_proc = [] (const auto& exp, auto env) {
    if (auto rator = value_of(exp.rator, env);
        type_of(rator) == ValueType::PROC) {

      auto& proc = value_to_proc(rator);
      auto args = value_of(exp.rands, env);
      auto new_env = Env::extend(proc.saved_env, proc.vars, std::move(args));
      return value_of(proc.body, new_env);
    } else {
      std::string msg = "the rator should be a Proc object";
      throw std::runtime_error(msg);
    }
  };

  if (type_of(exp.rator) == ExprType::VAR_EXP) {
    auto& sym = to_var_exp(exp.rator).var;
    auto f_opt = built_in::find_built_in(sym);
    if (f_opt) {
      auto args = value_of(exp.rands, env);
      return (*f_opt)(args);
    } else {
      return eval_proc(exp, env);
    }
  } else {
    return eval_proc(exp, env);
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