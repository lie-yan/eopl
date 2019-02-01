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
#include <gsl/gsl>

namespace eopl {

template<typename T>
std::string error_message (const T& exp) {
  return fmt::format("{} should not appear in the program for value_of()",
                     boost::typeindex::type_id<T>().pretty_name());
}

Value value_of (const Program& program, const SpEnv& env, const SpStore& store) {
  return value_of(program.exp1, env, store);
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

std::vector<Value> refs_of (const std::vector<Value>& values, const SpStore& store) {
  std::vector<Value> refs;
  std::transform(std::begin(values), std::end(values),
                 std::back_inserter(refs),
                 [&store] (Value value) {
                   return store->newref(std::move(value));
                 });
  return refs;
}

//Value value_of (const NamelessVarExp& exp, const SpEnv& env, const SpStore& store) {
//  throw std::runtime_error(error_message(exp));
//}

Value value_of (const IfExp& exp, const SpEnv& env, const SpStore& store) {
  Value val1 = value_of(exp.cond, env, store);
  bool b1 = to_bool(val1).get();
  if (b1) return value_of(exp.then_, env, store);
  else return value_of(exp.else_, env, store);
}

Value value_of (const LetExp& exp, const SpEnv& env, const SpStore& store, std::true_type) {
  auto new_env = std::accumulate(std::begin(exp.clauses),
                                 std::end(exp.clauses),
                                 env,
                                 [&store] (SpEnv& acc, const LetExp::Clause& c) -> SpEnv {
                                   auto res = value_of(c.exp, acc, store);
                                   return Env::extend(std::move(acc),
                                                      c.var,
                                                      store->newref(std::move(res)));
                                 });
  return value_of(exp.body, new_env, store);
}

Value value_of (const LetExp& exp, const SpEnv& env, const SpStore& store, std::false_type) {

  std::vector<Symbol> vars;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(vars),
                 [] (const LetExp::Clause& c) -> Symbol {
                   return c.var;
                 });

  std::vector<Value> values;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(values),
                 [&env, &store] (const LetExp::Clause& c) -> Value {
                   return store->newref(value_of(c.exp, env, store));
                 });
  return value_of(exp.body,
                  Env::extend(env, std::move(vars), std::move(values)),
                  store);
}

Value value_of (const LetExp& exp, const SpEnv& env, const SpStore& store) {
  if (exp.star) {
    return value_of(exp, env, store, std::true_type());
  } else {
    return value_of(exp, env, store, std::false_type());
  }
}

//Value value_of (const NamelessLetExp& exp, const SpEnv& env, const SpStore& store) {
//  throw std::runtime_error(error_message(exp));
//}

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
    auto refs = refs_of(*values, store);
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

//Value value_of (const NamelessUnpackExp& exp, const SpEnv& env, const SpStore& store) {
//  throw std::runtime_error(error_message(exp));
//}

Value value_of (const ProcExp& exp, const SpEnv& env, const SpStore& store) {
  return to_value(Proc{exp.params, exp.body, env});
}

//Value value_of (const NamelessProcExp& exp, const SpEnv& env, const SpStore& store) {
//  throw std::runtime_error(error_message(exp));
//}



Value value_of (const CallExp& exp, const SpEnv& env, const SpStore& store) {

  if (type_of(exp.rator) != ExpType::VAR_EXP) {
    goto eval_proc;
  } else {
    const auto& op_name = to_var_exp(exp.rator).var;
    auto f_opt = built_in::find_built_in(op_name);
    if (!f_opt) {
      goto eval_proc;
    } else {
      auto args = value_of(exp.rands, env, store);
      return (*f_opt)(args, store);
    }
  }

eval_proc:
  if (auto rator = value_of(exp.rator, env, store);
      type_of(rator) == ValueType::PROC) {

    auto& proc = to_proc(rator);
    auto args = value_of(exp.rands, env, store);
    auto new_env = Env::extend(proc.saved_env(),
                               proc.params(),
                               refs_of(args, store));
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
  auto new_env = Env::extend(env, std::move(name_list), refs_of(saved, store));
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

SpEnv make_initial_env (const SpStore& store) {
  return
      Env::extend(
          Env::make_empty(),
          Symbol{"emptylist"},
          store->newref(to_value(Nil()))
      );
}

Value eval (const std::string& s) {
  std::istringstream ss(s);
  yy::Lexer lexer(ss);
  Program result;
  yy::parser p(lexer, result);
  p.set_debug_level(getenv("YYDEBUG") != nullptr);
  p.parse();

  auto store = Store::make_empty();
  return value_of(result, make_initial_env(store), store);
}


}