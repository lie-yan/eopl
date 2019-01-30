//
// Created by Lie Yan on 2019-01-27.
//

#include "interpreter.h"

#include "built_in.h"

#include "lex.yy.h"
#include "translator.h"

#include <numeric>
#include <fmt/ostream.h>
#include <boost/type_index.hpp>

namespace eopl {

template<typename T>
std::string error_message (const T& t) {
  return fmt::format("{} should not appear in the program for nameless_value_of()",
                     boost::typeindex::type_id<T>().pretty_name());
}

Value nameless_value_of (const Program& program, const SpNamelessEnv& nenv) {
  return nameless_value_of(program.exp1, nenv);
}

struct NamelessValueOfVisitor {
  const SpNamelessEnv& nenv;

  template<typename T>
  Value operator () (const boost::recursive_wrapper<T>& exp) { return nameless_value_of(exp.get(), nenv); }

  template<typename T>
  Value operator () (const T& exp) { return nameless_value_of(exp, nenv); }
};

Value nameless_value_of (const Expression& exp, const SpNamelessEnv& nenv) {
  return std::visit(NamelessValueOfVisitor{nenv}, *exp);
}

Value nameless_value_of (const ConstExp& exp, const SpNamelessEnv& nenv) {
  return to_value(exp.num);
}

Value nameless_value_of (const VarExp& exp, const SpNamelessEnv& nenv) {
  throw std::runtime_error(error_message(exp));
}

Value nameless_value_of (const NamelessVarExp& exp, const SpNamelessEnv& nenv) {
  return NamelessEnv::apply(nenv, exp.addr);
}

Value nameless_value_of (const IfExp& exp, const SpNamelessEnv& nenv) {
  Value val1 = nameless_value_of(exp.cond, nenv);
  bool b1 = to_bool(val1).get();
  if (b1) return nameless_value_of(exp.then_, nenv);
  else return nameless_value_of(exp.else_, nenv);
}

Value nameless_value_of (const LetExp& exp, const SpNamelessEnv& nenv) {
  throw std::runtime_error(error_message(exp));
}

Value nameless_value_of (const NamelessLetExp& exp, const SpNamelessEnv& nenv, std::false_type) {
  std::vector<Value> vals = nameless_value_of(exp.clauses, nenv);
  auto new_env = NamelessEnv::extend(nenv, std::move(vals));
  return nameless_value_of(exp.body, new_env);
}

Value nameless_value_of (const NamelessLetExp& exp, const SpNamelessEnv& nenv, std::true_type) {
  auto new_nenv = std::accumulate(std::begin(exp.clauses), std::end(exp.clauses),
                                  nenv,
                                  [] (SpNamelessEnv& acc, const Expression& c) {
                                    auto val = nameless_value_of(c, acc);
                                    return NamelessEnv::extend(std::move(acc), std::move(val));
                                  });
  return nameless_value_of(exp.body, new_nenv);
}

Value nameless_value_of (const NamelessLetExp& exp, const SpNamelessEnv& nenv) {
  if (exp.star) {
    return nameless_value_of(exp, nenv, std::true_type());
  } else {
    return nameless_value_of(exp, nenv, std::false_type());
  }
}

Value nameless_value_of (const CondExp& exp, const SpNamelessEnv& nenv) {
  auto it = std::find_if(std::begin(exp.clauses),
                         std::end(exp.clauses),
                         [nenv] (const CondExp::Clause& c) -> bool {
                           auto b = nameless_value_of(c.cond, nenv);
                           return to_bool(b).get();
                         });
  if (it == std::end(exp.clauses)) {
    throw std::runtime_error("at least one clause should be true for the "
                             "cond expression, but none were found");
  } else {
    return nameless_value_of(it->body, nenv);
  }
}

Value nameless_value_of (const UnpackExp& exp, const SpNamelessEnv& nenv) {
  throw std::runtime_error(error_message(exp));
}

Value nameless_value_of (const NamelessUnpackExp& exp, const SpNamelessEnv& nenv) {
  Value pack = nameless_value_of(exp.pack, nenv);
  std::vector<Value> unpacked = flatten(pack);

  if (unpacked.size() == exp.var_num) {
    return nameless_value_of(exp.body,
                             NamelessEnv::extend(nenv, std::move(unpacked)));
  } else {
    throw std::runtime_error("the size of identifier list and that of the pack "
                             "does not match");
  }

}

Value nameless_value_of (const ProcExp& exp, const SpNamelessEnv& nenv) {
  throw std::runtime_error(error_message(exp));
}

Value nameless_value_of (const NamelessProcExp& exp, const SpNamelessEnv& nenv) {
  return to_value(NamelessProc{exp.body, nenv});
}

Value nameless_value_of (const CallExp& exp, const SpNamelessEnv& nenv) {
  // TODO: refactor the processing of CallExp
  auto eval_proc = [] (const CallExp& exp, auto env) {
    if (auto rator = nameless_value_of(exp.rator, env);
        type_of(rator) == ValueType::NAMELESS_PROC) {

      auto& proc = to_nameless_proc(rator);
      auto args = nameless_value_of(exp.rands, env);

      auto new_env = NamelessEnv::extend(proc.saved_env(), std::move(args));
      return nameless_value_of(proc.body(), new_env);
    } else {
      std::string msg = "the rator should be a NamelessProc object";
      throw std::runtime_error(msg);
    }
  };

  if (type_of(exp.rator) == ExpType::VAR_EXP) {
    auto& op_name = to_var_exp(exp.rator).var;
    auto f_opt = built_in::find_built_in(op_name);
    if (f_opt) {
      auto args = nameless_value_of(exp.rands, nenv);
      return (*f_opt)(args);
    } else {
      throw std::runtime_error("this should not happen");
    }
  } else {
    return eval_proc(exp, nenv);
  }
}

Value nameless_value_of (const LetrecExp& exp, const SpNamelessEnv& nenv) {
  throw std::runtime_error(error_message(exp));
}

Value nameless_value_of (const NamelessLetrecExp& exp, const SpNamelessEnv& nenv) {
  std::vector<Value> saved;

  std::transform(std::begin(exp.procs), std::end(exp.procs),
                 std::back_inserter(saved),
                 [] (const NamelessLetrecProcSpec& spec) {
                   return to_value(NamelessProc{spec.body, {}});
                 });
  auto new_nenv = NamelessEnv::extend(nenv, saved);
  for (auto& v : saved) to_nameless_proc(v).saved_env(new_nenv);

  return nameless_value_of(exp.body, new_nenv);
}

std::vector<Value> nameless_value_of (const std::vector<Expression>& exp_list, const SpNamelessEnv& nenv) {
  std::vector<Value> values;
  std::transform(std::begin(exp_list), std::end(exp_list),
                 std::back_inserter(values),
                 [&nenv] (const Expression& e) {
                   return nameless_value_of(e, nenv);
                 });
  return values;
}

SpNamelessEnv make_initial_nenv () {
  return
      NamelessEnv::extend(
          NamelessEnv::make_empty(),
          to_value(Nil())
      );
}

Value nameless_eval (const std::string& s) {
  std::istringstream ss(s);
  yy::Lexer lexer(ss);
  Program result;
  yy::parser p(lexer, result);
  p.set_debug_level(getenv("YYDEBUG") != nullptr);
  p.parse();

  Program program = translation_of(result, make_initial_senv());
  return nameless_value_of(program, make_initial_nenv());
}

}