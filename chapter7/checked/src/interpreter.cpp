//
// Created by robin on 2019-01-10.
//

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <numeric>
#include <utility>

#include "interpreter.h"
#include "value.h"
#include "built_in.h"
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
    Value operator () (const RwCondExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwUnpackExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwProcExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwCallExp& exp) { return value_of(exp.get(), env); }
    Value operator () (const RwLetrecExp& exp) { return value_of(exp.get(), env); }
  };
  return std::visit(EvalVisitor{env}, *exp);
}

Value value_of (const ConstExp& exp, SpEnv env) {
  return to_value(exp.num);
}

Value value_of (const VarExp& exp, SpEnv env) {
  return Env::apply(std::move(env), exp.var);
}

Value value_of (const IfExp& exp, SpEnv env) {
  Value val1 = value_of(exp.cond, env);
  bool b1 = to_bool(val1).get();
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

Value value_of (const CondExp& exp, SpEnv env) {
  auto it = std::find_if(std::begin(exp.clauses),
                         std::end(exp.clauses),
                         [env] (const CondExp::Clause& c) -> bool {
                           auto b = value_of(c.first, env);
                           return to_bool(b).get();
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
                            auto& pair = to_pair(acc.first);
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
  return to_value(Proc{exp.params, exp.body, std::move(env)});
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

      auto& proc = to_proc(rator);
      auto args = value_of(exp.rands, env);

      auto new_env = Env::extend(proc.saved_env, proc.params, std::move(args));
      return value_of(proc.body, new_env);
    } else {
      std::string msg = "the rator should be a Proc object";
      throw std::runtime_error(msg);
    }
  };

  if (type_of(exp.rator) == ExpType::VAR_EXP) {
    auto& op_name = to_var_exp(exp.rator).var;
    auto f_opt = built_in::find_fun(op_name);
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

Value value_of (const LetrecExp& exp, SpEnv env) {
  std::vector<Value> saved;
  SpEnv new_env = std::accumulate(std::begin(exp.procs),
                                  std::end(exp.procs),
                                  std::move(env),
                                  [&saved] (SpEnv acc, const LetrecProc& proc) {
                                    auto p = to_value(Proc{proc.params, proc.body, acc});
                                    saved.push_back(p);
                                    return Env::extend(acc, proc.name, p);
                                  });
  for (auto& v : saved) to_proc(v).saved_env = new_env;
  return value_of(exp.body, new_env);
}

SpEnv make_initial_env () {
  auto ret = Env::make_empty();
  ret = Env::extend(ret, Symbol{"emptylist"}, to_value(Nil()));
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