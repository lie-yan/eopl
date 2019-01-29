//
// Created by robin on 2019-01-10.
//

#include "interpreter.h"

#include "built_in.h"
#include "lex.yy.h"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <numeric>
#include <boost/type_index.hpp>

namespace eopl {

template<typename T>
std::string error_message (const T& exp) {
  return fmt::format("{} should not appear in the program for value_of()",
                     boost::typeindex::type_id<T>().pretty_name());
}

Value value_of (const Program& program, const SpEnv& env) {
  return value_of(program.exp1, env);
}

struct ValueOfVisitor {
  const SpEnv& env;

  template<typename T>
  Value operator () (const boost::recursive_wrapper<T>& exp) { return value_of(exp.get(), env); }

  template<typename T>
  Value operator () (const T& exp) { return value_of(exp, env); }
};

Value value_of (const Expression& exp, const SpEnv& env) {
  return std::visit(ValueOfVisitor{env}, *exp);
}

Value value_of (const ConstExp& exp, const SpEnv& env) {
  return to_value(exp.num);
}

Value value_of (const VarExp& exp, const SpEnv& env) {
  return Env::apply(env, exp.var);
}

Value value_of (const NamelessVarExp& exp, const SpEnv& env) {
  throw std::runtime_error(error_message(exp));
}

Value value_of (const IfExp& exp, const SpEnv& env) {
  Value val1 = value_of(exp.cond, env);
  bool b1 = to_bool(val1).get();
  if (b1) return value_of(exp.then_, env);
  else return value_of(exp.else_, env);
}

Value value_of (const LetExp& exp, const SpEnv& env, std::true_type) {
  auto new_env = std::accumulate(std::begin(exp.clauses),
                                 std::end(exp.clauses),
                                 env,
                                 [] (SpEnv acc, const LetExp::Clause& c) -> SpEnv {
                                   auto res = value_of(c.second, acc);
                                   return Env::extend(std::move(acc), c.first, std::move(res));
                                 });
  return value_of(exp.body, new_env);
}

Value value_of (const LetExp& exp, const SpEnv& env, std::false_type) {
  auto new_env = std::accumulate(std::begin(exp.clauses),
                                 std::end(exp.clauses),
                                 env,
                                 [&env] (SpEnv acc, const LetExp::Clause& c) -> SpEnv {
                                   auto res = value_of(c.second, env);
                                   return Env::extend(std::move(acc), c.first, std::move(res));
                                 });
  return value_of(exp.body, new_env);
}

Value value_of (const LetExp& exp, const SpEnv& env) {
  if (exp.star) {
    return value_of(exp, env, std::true_type());
  } else {
    return value_of(exp, env, std::false_type());
  }
}

Value value_of (const NamelessLetExp& exp, const SpEnv& env) {
  throw std::runtime_error(error_message(exp));
}

Value value_of (const CondExp& exp, const SpEnv& env) {
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

Value value_of (const UnpackExp& exp, const SpEnv& env) {
  static const std::string msg = "the size of identifier list and that of the pack "
                                 "does not match";

  auto lst = value_of(exp.pack, env);
  using P = decltype(std::pair(lst, env));
  P p = std::accumulate(std::begin(exp.vars),
                        std::end(exp.vars),
                        P(lst, env),
                        [] (P acc, const Symbol& s) -> P {
                          if (type_of(acc.first) == ValueType::PAIR) {
                            auto& pair = to_pair(acc.first);
                            auto new_env = Env::extend(std::move(acc.second), s, pair.first);
                            return std::pair(pair.second, std::move(new_env));
                          } else {
                            throw std::runtime_error(msg);
                          }
                        });
  if (type_of(p.first) != ValueType::NIL) {
    throw std::runtime_error(msg);
  } else {
    return value_of(exp.body, p.second);
  }
}

Value value_of (const NamelessUnpackExp& exp, const SpEnv& env) {
  throw std::runtime_error(error_message(exp));
}

Value value_of (const ProcExp& exp, const SpEnv& env) {
  return to_value(Proc{exp.params, exp.body, env});
}

Value value_of (const NamelessProcExp& exp, const SpEnv& env) {
  throw std::runtime_error(error_message(exp));
}

std::vector<Value> value_of (const std::vector<Expression>& exps, const SpEnv& env) {
  std::vector<Value> results;
  std::transform(std::begin(exps),
                 std::end(exps),
                 std::back_inserter(results),
                 [&env] (const auto& e) -> Value {
                   return value_of(e, env);
                 });
  return results;
}

Value value_of (const CallExp& exp, const SpEnv& env) {

  auto eval_proc = [] (const auto& exp, auto env) {
    if (auto rator = value_of(exp.rator, env);
        type_of(rator) == ValueType::PROC) {

      auto& proc = to_proc(rator);
      auto args = value_of(exp.rands, env);

      auto new_env = Env::extend(proc.saved_env(), std::move(proc.params()), std::move(args));
      return value_of(proc.body(), new_env);
    } else {
      std::string msg = "the rator should be a Proc object";
      throw std::runtime_error(msg);
    }
  };

  if (type_of(exp.rator) == ExpType::VAR_EXP) {
    auto& op_name = to_var_exp(exp.rator).var;
    auto f_opt = built_in::find_built_in(op_name);
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

Value value_of (const LetrecExp& exp, const SpEnv& env) {
  std::vector<Value> saved;
  SpEnv new_env = std::accumulate(std::begin(exp.proc_list),
                                  std::end(exp.proc_list),
                                  env,
                                  [&saved] (const SpEnv& acc, const LetrecProcSpec& proc) {
                                    auto p = to_value(Proc{proc.params, proc.body, acc});
                                    saved.push_back(p);
                                    return Env::extend(acc, proc.name, p);
                                  });
  for (auto& v : saved) to_proc(v).saved_env(new_env);
  return value_of(exp.body, new_env);
}

Value value_of (const NamelessLetrecExp& exp, const SpEnv& env) {
  throw std::runtime_error(error_message(exp));
}

SpEnv make_initial_env () {
  return
      Env::extend(
          Env::make_empty(),
          Symbol{"emptylist"},
          to_value(Nil())
      );
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