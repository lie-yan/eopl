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
                                 [] (SpEnv& acc, const LetExp::Clause& c) -> SpEnv {
                                   auto res = value_of(c.exp, acc);
                                   return Env::extend(std::move(acc), c.var, std::move(res));
                                 });
  return value_of(exp.body, new_env);
}

Value value_of (const LetExp& exp, const SpEnv& env, std::false_type) {
  auto new_env = std::accumulate(std::begin(exp.clauses),
                                 std::end(exp.clauses),
                                 env,
                                 [&env] (SpEnv& acc, const LetExp::Clause& c) -> SpEnv {
                                   auto res = value_of(c.exp, env);
                                   return Env::extend(std::move(acc), c.var, std::move(res));
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
                           auto b = value_of(c.cond, env);
                           return to_bool(b).get();
                         });
  if (it == std::end(exp.clauses)) {
    throw std::runtime_error("at least one clause should be true for the "
                             "cond expression, but none were found");
  } else {
    return value_of(it->body, env);
  }
}

Value value_of (const UnpackExp& exp, const SpEnv& env) {

  auto lst = value_of(exp.pack, env);
  std::optional<std::vector<Value>> values = flatten(lst);

  if (!values) {
    throw std::runtime_error("list expected");
  } else if (values->size() == exp.vars.size()) {
    return value_of(exp.body,
                    Env::extend(env, exp.vars, std::move(*values)));
  } else {
    throw std::runtime_error("the size of identifier list and that of the pack "
                             "does not match");
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

  if (type_of(exp.rator) != ExpType::VAR_EXP) {
    goto eval_proc;
  } else {
    const auto& op_name = to_var_exp(exp.rator).var;
    auto f_opt = built_in::find_built_in(op_name);
    if (!f_opt) {
      goto eval_proc;
    } else {
      auto args = value_of(exp.rands, env);
      return (*f_opt)(args);
    }
  }

eval_proc:
  if (auto rator = value_of(exp.rator, env);
      type_of(rator) == ValueType::PROC) {

    auto& proc = to_proc(rator);
    auto args = value_of(exp.rands, env);

    auto new_env = Env::extend(proc.saved_env(),
                               proc.params(),
                               std::move(args));
    return value_of(proc.body(), new_env);
  } else {
    std::string msg = "the rator should be a Proc object";
    throw std::runtime_error(msg);
  }
}

Value value_of (const LetrecExp& exp, const SpEnv& env) {
  std::vector<Value> saved;
  SpEnv new_env = std::accumulate(std::begin(exp.proc_list),
                                  std::end(exp.proc_list),
                                  env,
                                  [&saved] (SpEnv& acc, const LetrecProcSpec& proc) {
                                    auto p = to_value(Proc{proc.params, proc.body, acc});
                                    saved.push_back(p);
                                    return Env::extend(std::move(acc), proc.name, p);
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