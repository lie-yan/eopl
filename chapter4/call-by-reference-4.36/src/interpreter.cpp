//
// Created by robin on 2019-01-10.
//

#include "interpreter.h"

#include "built_in.h"
#include "stmt.h"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <numeric>
#include <gsl/gsl>
#include <iostream>
#include "parser.tab.hpp"
#include "lex.yy.h"

namespace eopl {

template<typename T>
std::string error_message (const T& exp) {
  return fmt::format("{} should not appear in the program for value_of()",
                     boost::typeindex::type_id<T>().pretty_name());
}

struct ValueOfVisitor {
  const SpEnv& env;
  const SpStore& store;

  template<typename T>
  Value operator () (const boost::recursive_wrapper<T>& exp) { return value_of(exp.get(), env, store); }

  template<typename T>
  Value operator () (const T& exp) { return value_of(exp, env, store); }
};

Value value_of (const Expression& exp, const SpEnv& env, const SpStore& store) {
  return std::visit(ValueOfVisitor{env, store}, *exp);
}

Value value_of (const ConstExp& exp, const SpEnv& env, const SpStore& store) {
  return to_value(exp.num);
}

Value value_of (const VarExp& exp, const SpEnv& env, const SpStore& store) {
  return
      store->deref(
          Env::apply(env, exp.var)
      );
}

std::vector<Value> value_of (const std::vector<Expression>& exps, const SpEnv& env, const SpStore& store) {
  std::vector<Value> results;
  std::transform(std::begin(exps),
                 std::end(exps),
                 std::back_inserter(results),
                 [&env, &store] (const auto& e) -> Value {
                   return value_of(e, env, store);
                 });
  return results;
}

std::vector<Ref> new_refs_of (const std::vector<Value>& values, const SpStore& store) {
  std::vector<Ref> refs;
  std::transform(std::begin(values), std::end(values),
                 std::back_inserter(refs),
                 [&store] (Value value) {
                   return store->newref(std::move(value));
                 });
  return refs;
}

Value value_of (const IfExp& exp, const SpEnv& env, const SpStore& store) {
  Value val1 = value_of(exp.cond, env, store);
  bool b1 = to_bool(val1).get();
  if (b1) return value_of(exp.then_, env, store);
  else return value_of(exp.else_, env, store);
}

struct let_tag { };
struct let_star_tag { };

Value value_of (const LetExp& exp, const SpEnv& env, const SpStore& store, let_star_tag) {
  auto binop_ref = [&store] (SpEnv& acc, const BindingClause& c) -> SpEnv {
    auto res = value_of_operand(c.exp, acc, store);
    return Env::extend(std::move(acc),
                       c.var,
                       res);
  };

  auto binop_value = [&store] (SpEnv& acc, const BindingClause& c) -> SpEnv {
    auto res = value_of(c.exp, acc, store);
    return Env::extend(std::move(acc),
                       c.var,
                       store->newref(std::move(res)));
  };

  if (exp.ref) {
    auto new_env = std::accumulate(std::begin(exp.clauses),
                                   std::end(exp.clauses),
                                   env,
                                   binop_ref);
    return value_of(exp.body, new_env, store);
  } else {
    auto new_env = std::accumulate(std::begin(exp.clauses),
                                   std::end(exp.clauses),
                                   env,
                                   binop_value);
    return value_of(exp.body, new_env, store);
  }
}

Value value_of (const LetExp& exp, const SpEnv& env, const SpStore& store, let_tag) {

  std::vector<Symbol> vars;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(vars),
                 [] (const BindingClause& c) -> Symbol {
                   return c.var;
                 });

  std::vector<Ref> refs;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(refs),
                 [&env, &store, &exp] (const BindingClause& c) -> Ref {
                   if (exp.ref)
                     return value_of_operand(c.exp, env, store);
                   else
                     return store->newref(value_of(c.exp, env, store));
                 });
  return value_of(exp.body,
                  Env::extend(env, std::move(vars), std::move(refs)),
                  store);
}

Value value_of (const LetExp& exp, const SpEnv& env, const SpStore& store) {
  if (exp.star) {
    return value_of(exp, env, store, let_star_tag());
  } else {
    return value_of(exp, env, store, let_tag());
  }
}

Value value_of (const CondExp& exp, const SpEnv& env, const SpStore& store) {
  auto it = std::find_if(std::begin(exp.clauses),
                         std::end(exp.clauses),
                         [env, store] (const CondExp::Clause& c) -> bool {
                           auto b = value_of(c.cond, env, store);
                           return to_bool(b).get();
                         });
  if (it == std::end(exp.clauses)) {
    throw std::runtime_error("at least one clause should be true for the "
                             "cond expression, but none were found");
  } else {
    return value_of(it->body, env, store);
  }
}

Value value_of (const UnpackExp& exp, const SpEnv& env, const SpStore& store) {

  auto lst = value_of(exp.pack, env, store);
  std::optional<std::vector<Value>> values = flatten(lst);

  if (values && values->size() == exp.vars.size()) {
    auto refs = new_refs_of(*values, store);
    return value_of(exp.body,
                    Env::extend(env, exp.vars, std::move(refs)),
                    store);
  } else if (!values) {
    throw std::runtime_error("list expected");
  } else {
    throw std::runtime_error("the size of identifier list and that of the pack "
                             "does not match");
  }

}

Value value_of (const ProcExp& exp, const SpEnv& env, const SpStore& store) {
  return to_value(Proc{exp.params, exp.body, env});
}

Value value_of (const SubrExp& exp, const SpEnv& env, const SpStore& store) {
  return to_value(Subr{exp.params, exp.body, env});
}

Value value_of (const CallExp& exp, const SpEnv& env, const SpStore& store) {

  if (type_of(exp.rator) != ExpType::VAR_EXP) {
    goto eval_proc;
  } else {
    const auto& op_name = to_var_exp(exp.rator).var;
    auto f_opt = built_in::find_function(op_name);
    if (!f_opt) {
      goto eval_proc;
    } else {
      auto args_ref = value_of_operands(exp.rands, env, store);
      std::vector<Value> args;
      std::transform(std::begin(args_ref), std::end(args_ref),
                     std::back_inserter(args),
                     [&store] (const Ref& ref) { return store->deref(ref); });
      return (*f_opt)(args, store);
    }
  }

eval_proc:
  if (auto rator = value_of(exp.rator, env, store);
      type_of(rator) == ValueType::PROC) {

    auto& proc = to_proc(rator);
    auto args = value_of_operands(exp.rands, env, store);
    auto new_env = Env::extend(proc.saved_env(),
                               proc.params(),
                               args);
    return value_of(proc.body(), new_env, store);
  } else {
    throw std::runtime_error("the rator should be a Proc object");
  }
}

Value value_of (const LetrecExp& exp, const SpEnv& env, const SpStore& store) {
  std::vector<Value> saved;
  std::transform(std::begin(exp.proc_list),
                 std::end(exp.proc_list),
                 std::back_inserter(saved),
                 [&store, &env] (const LetrecProcSpec& proc) {
                   return to_value(Proc{proc.params, proc.body, env});
                 });
  std::vector<Symbol> name_list;
  std::transform(std::begin(exp.proc_list), std::end(exp.proc_list),
                 std::back_inserter(name_list),
                 [] (const LetrecProcSpec& proc) {
                   return proc.name;
                 });
  auto new_env = Env::extend(env, std::move(name_list), new_refs_of(saved, store));
  for (auto& v : saved) to_proc(v).saved_env(new_env);
  return value_of(exp.body, new_env, store);
}

Value value_of (const SequenceExp& exp, const SpEnv& env, const SpStore& store) {
  std::vector<Value> results;
  std::transform(std::begin(exp.exp_list), std::end(exp.exp_list), std::back_inserter(results),
                 [&env, &store] (const Expression& expression) {
                   return value_of(expression, env, store);
                 });
  return results.back();
}

Value value_of (const AssignExp& exp, const SpEnv& env, const SpStore& store) {
  store->setref(
      Env::apply(env, exp.var),
      value_of(exp.exp1, env, store)
  );
  return to_value(Int{27});
}

Value value_of (const SetdynamicExp& exp, const SpEnv& env, const SpStore& store) {
  struct ShadowedPair {
    Ref ref;
    Value value;
  };

  std::vector<ShadowedPair> shadowed;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(shadowed),
                 [&env, &store] (const BindingClause& c) {
                   auto old_ref = Env::apply(env, c.var);
                   auto old_value = store->deref(old_ref);
                   auto new_value = value_of(c.exp, env, store);
                   store->setref(old_ref, new_value);
                   return ShadowedPair{old_ref, old_value};
                 });
  auto ret_value = value_of(exp.body, env, store);

  std::for_each(std::begin(shadowed), std::end(shadowed),
                [&store] (const ShadowedPair& p) {
                  store->setref(p.ref, p.value);
                });
  return ret_value;
}

std::vector<Ref> value_of_operands (const std::vector<Expression>& exps, const SpEnv& env, const SpStore& store) {
  std::vector<Ref> ret;

  std::transform(std::begin(exps), std::end(exps),
                 std::back_inserter(ret),
                 [&env, &store] (const Expression& exp) {
                   return value_of_operand(exp, env, store);
                 });

  return ret;
}

bool is_arrayref_call (const Expression& exp) {
  if (type_of(exp) != ExpType::CALL_EXP) return false;
  auto&rator = to_call_exp(exp).rator;
  if (type_of(rator) != ExpType::VAR_EXP) return false;
  auto& name = to_var_exp(rator).var;
  return name == Symbol{"arrayref"};
}

Ref value_of_operand (const Expression& exp, const SpEnv& env, const SpStore& store) {
  if (auto type = type_of(exp); type == ExpType::VAR_EXP) {
    return Env::apply(env, to_var_exp(exp).var);
  } else if (is_arrayref_call(exp)){
    auto args_ref = value_of_operands(to_call_exp(exp).rands, env, store);
    std::vector<Value> args;
    std::transform(std::begin(args_ref), std::end(args_ref),
                   std::back_inserter(args),
                   [&store] (const Ref& ref) { return store->deref(ref); });
    return built_in::arrayref_r(args, store);
  } else {
    auto value = value_of(exp, env, store);
    return store->newref(value);
  }
}

void result_of (const Program& program, const SpEnv& env, const SpStore& store) {
  result_of(program.stmt, env, store);
}

struct ResultOfStatementVisitor {
  const SpEnv& env;
  const SpStore& store;

  template<typename T>
  void operator () (const boost::recursive_wrapper<T>& t) { result_of(t.get(), env, store); }

  template<typename T>
  void operator () (const T& t) { result_of(t, env, store); }
};

void result_of (const Statement& statement, const SpEnv& env, const SpStore& store) {
  std::visit(ResultOfStatementVisitor{env, store}, *statement);
}

void result_of (const AssignStmt& statement, const SpEnv& env, const SpStore& store) {
  Ref ref = Env::apply(env, statement.var);
  Value value = value_of(statement.exp, env, store);
  store->setref(ref, value);
}

void result_of (const SubrCallStmt& statement, const SpEnv& env, const SpStore& store) {
  auto subr_opt = built_in::find_subroutine(statement.var);
  if (subr_opt) {
    std::vector<Value> args = value_of(statement.params, env, store);
    (*subr_opt)(args, store);
  } else {
    auto subr_ref = Env::apply(env, statement.var);
    const Subr& subr = to_subr(store->deref(subr_ref));

    auto args = value_of_operands(statement.params, env, store);
    auto new_env = Env::extend(subr.saved_env(), subr.params(), std::move(args));
    result_of(subr.body(), new_env, store);
  }
}

void result_of (const BlockStmt& statement, const SpEnv& env, const SpStore& store) {
  std::for_each(
      std::begin(statement.stmt_list),
      std::end(statement.stmt_list),
      [&env, &store] (const Statement& stmt) {
        result_of(stmt, env, store);
      });
}

void result_of (const IfStmt& statement, const SpEnv& env, const SpStore& store) {
  Value cond = value_of(statement.cond, env, store);
  if (to_bool(cond).get()) {
    result_of(statement.then_, env, store);
  } else {
    result_of(statement.else_, env, store);
  }
}

void result_of (const WhileStmt& statement, const SpEnv& env, const SpStore& store) {
  while (true) {
    Value cond = value_of(statement.cond, env, store);
    if (to_bool(cond).get()) {
      result_of(statement.body, env, store);
    } else {
      break;
    }
  }
}

void result_of (const DoWhileStmt& statement, const SpEnv& env, const SpStore& store) {
  while (true) {
    result_of(statement.body, env, store);
    Value cond = value_of(statement.cond, env, store);
    if (!to_bool(cond).get()) break;
  }
}

void result_of (const DeclStmt& statement, const SpEnv& env, const SpStore& store) {
  std::vector<Ref> new_refs;
  std::vector<Symbol> vars;
  std::vector<Value> saved;
  std::for_each(std::begin(statement.vars),
                std::end(statement.vars),
                [&env, &store, &vars, &new_refs, &saved] (const VarDeclClause& c) {
                  vars.push_back(c.var);
                  if (c.initializer) {
                    Value value = value_of(*c.initializer, env, store);
                    if (auto type = type_of(value); type == ValueType::PROC) {
                      saved.push_back(value);
                    }
                    new_refs.push_back(store->newref(std::move(value)));
                  } else {
                    new_refs.push_back(store->newref(to_value(Int{0})));
                  }
                });
  auto new_env = Env::extend(env, vars, new_refs);
  std::for_each(std::begin(saved), std::end(saved),
                [&new_env] (Value& value) {
                  to_proc(value).saved_env(new_env);
                });
  result_of(statement.body, new_env, store);
}

void result_of (const ReadStmt& statement, const SpEnv& env, const SpStore& store) {
  Ref ref = Env::apply(env, statement.var);
  int i = 0;
  std::cin >> i;
  store->setref(ref, to_value(Int{i}));
}

SpEnv make_initial_env (const SpStore& store) {
  return
      Env::extend(
          Env::make_empty(),
          Symbol{"emptylist"},
          store->newref(to_value(Nil()))
      );
}

void run (const std::string& s) {
  std::istringstream ss(s);
  yy::Lexer lexer(ss);
  Program result;
  yy::parser p(lexer, result);
  p.set_debug_level(getenv("YYDEBUG") != nullptr);
  p.parse();

  auto store = Store::make_empty();
  auto env = make_initial_env(store);

  result_of(result, env, store);
}


}