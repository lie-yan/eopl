//
// Created by Lie Yan on 2019-02-10.
//

#include "checker.h"

#include "type.h"
#include "expr.h"

#include "parser.tab.hpp"
#include "lex.yy.h"
#include <gsl/gsl>
#include <fmt/ostream.h>

namespace eopl::type {

Type type_of (const Program& program, const SpTEnv& tenv) {
  return type_of(program.exp1, make_initial_tenv());
}

std::vector<Type> get_arg_types (const std::vector<ParamDecl>& decl_list) {
  return transform(decl_list,
                   [] (const ParamDecl& decl) {
                     return decl.ty;
                   });
}

std::vector<Symbol> get_arg_names (const std::vector<ParamDecl>& decl_list) {
  return transform(decl_list,
                   [] (const ParamDecl& decl) {
                     return decl.sym;
                   });
}

struct TypeCheckVisitor {
  const SpTEnv& tenv;

  template<typename T>
  Type operator () (const boost::recursive_wrapper<T>& t) { return type_of(t.get(), tenv); }

  template<typename T>
  Type operator () (const T& t) { return type_of(t, tenv); }
};

Type type_of (const Expression& expression, const SpTEnv& tenv) {
  return std::visit(TypeCheckVisitor{tenv}, *expression);
}

Type type_of (const ConstExp& exp, const SpTEnv& tenv) {
  return to_type(IntType{});
}

Type type_of (const VarExp& exp, const SpTEnv& tenv) {
  return TEnv::apply(tenv, exp.var);
}

Type type_of (const IfExp& exp, const SpTEnv& tenv) {
  Type cond_type = type_of(exp.cond, tenv);
  check_equal_type(cond_type, to_type(BoolType{}), exp.cond);
  Type then_type = type_of(exp.then_, tenv);
  Type else_type = type_of(exp.else_, tenv);
  check_equal_type(else_type, then_type, exp.else_);
  return then_type;
}

Type type_of (const LetExp& exp, const SpTEnv& tenv, std::true_type) {
  Expects(exp.star);
  auto new_tenv = std::accumulate(std::begin(exp.clauses),
                                  std::end(exp.clauses),
                                  tenv,
                                  [] (const SpTEnv& acc, const LetExp::Clause& c) {
                                    auto var_type = type_of(c.second, acc);
                                    return TEnv::extend(acc, c.first, var_type);
                                  });
  return type_of(exp.body, new_tenv);
}

Type type_of (const LetExp& exp, const SpTEnv& tenv, std::false_type) {
  Expects(!exp.star);
  std::vector<Type> var_types = transform(exp.clauses,
                                          [&tenv] (const LetExp::Clause& c) {
                                            return type_of(c.second, tenv);
                                          });
  std::vector<Symbol> symbols = transform(exp.clauses,
                                          [] (const LetExp::Clause& c) {
                                            return c.first;
                                          });
  SpTEnv new_env = TEnv::extend(tenv, symbols, var_types);
  Type result_type = type_of(exp.body, new_env);
  return result_type;
}

Type type_of (const LetExp& exp, const SpTEnv& tenv) {
  if (exp.star) return type_of(exp, tenv, std::true_type());
  else return type_of(exp, tenv, std::false_type());
}

Type type_of (const CondExp& exp, const SpTEnv& tenv) {

  std::vector<Type> branch_types
      = transform(exp.clauses,
                  [&tenv] (const CondExp::Clause& c) {
                    Type cond_type = type_of(c.first, tenv);
                    check_equal_type(cond_type, to_type(BoolType()), c.first);
                    Type branch_type = type_of(c.second, tenv);
                    return branch_type;
                  });

  if (branch_types.empty()) {
    throw std::runtime_error("CondExp should contain at least one branch");
  } else {
    bool all_same = std::all_of(std::next(std::begin(branch_types)),
                                std::end(branch_types),
                                [t = branch_types[0]] (const Type& other) -> bool {
                                  return t == other;
                                });
    if (all_same) {
      return branch_types[0];
    } else {
      throw std::runtime_error("The types of all branches should be the same.");
    }
  }
}

Type type_of (const UnpackExp& exp, const SpTEnv& tenv) {
  return Type();
}

Type type_of (const ProcExp& exp, const SpTEnv& tenv) {
  std::vector<Symbol> vars = get_arg_names(exp.params);
  std::vector<Type> var_types = get_arg_types(exp.params);
  auto new_tenv = TEnv::extend(tenv, vars, var_types);
  auto result_type = type_of(exp.body, new_tenv);
  return to_type(ProcType{std::move(var_types), std::move(result_type)});
}

Type type_of (const CallExp& exp, const SpTEnv& tenv) {
  auto rator_type = type_of(exp.rator, tenv);
  auto rand_types = type_of(exp.rands, tenv);

  if (type_of(rator_type) == TypeEnum::ProcType) {
    const ProcType& p = to_proc_type(rator_type);

    auto[first1, first2]
    = std::mismatch(
        std::begin(rand_types), std::end(rand_types),
        std::begin(exp.rands),
        [&tenv] (const Type& t, const Expression& e) -> bool {
          Type other = type_of(e, tenv);
          check_equal_type(other, t, e);
          return true;
        });

    Expects(first1 == std::end(rand_types) && first2 == std::end(exp.rands));
    return p.result_type;
  } else {
    report_on_rator_not_proc(rator_type, exp.rator);
    return {};
  }
}

Type type_of (const LetrecExp& exp, const SpTEnv& tenv) {
  std::vector<Type> proc_types
      = transform(exp.procs,
                  [&tenv] (const LetrecProc& p) -> Type {
                    std::vector<Type> var_types = get_arg_types(p.params);
                    return to_type(ProcType{var_types, p.result_type});
                  });
  std::vector<Symbol> proc_names
      = transform(exp.procs,
                  [] (const LetrecProc& p) {
                    return p.name;
                  });
  auto new_tenv = TEnv::extend(tenv, proc_names, proc_types);

  std::all_of(std::begin(exp.procs), std::end(exp.procs),
              [&new_tenv] (const LetrecProc& p) -> bool {
                auto proc_tenv = TEnv::extend(new_tenv,
                                              get_arg_names(p.params),
                                              get_arg_types(p.params));
                auto body_type = type_of(p.body, proc_tenv);
                check_equal_type(body_type, p.result_type, p.body);
                return true;
              });
  return type_of(exp.body, new_tenv);
}

std::vector<Type> type_of (const std::vector<Expression>& exp_list, const SpTEnv& tenv) {
  return transform(exp_list,
                   [&tenv] (const Expression& expression) {
                     return type_of(expression, tenv);
                   });
}

SpTEnv make_initial_tenv () {
  auto tenv = TEnv::make_empty();
  auto unary_bool
      = to_type(
          ProcType{
              std::vector<Type>{to_type(IntType{})},
              to_type(BoolType{})});

  tenv = TEnv::extend(tenv,
                      Symbol{"zero?"},
                      unary_bool);

  auto binary_bool
      = to_type(
          ProcType{
              std::vector<Type>{to_type(IntType{}), to_type(IntType{})},
              to_type(BoolType{})});

  tenv = TEnv::extend(tenv,
                      Symbol{"equal?"},
                      binary_bool);
  tenv = TEnv::extend(tenv,
                      Symbol{"greater?"},
                      binary_bool);
  tenv = TEnv::extend(tenv,
                      Symbol{"less?"},
                      binary_bool);

  auto unary_int
      = to_type(
          ProcType{
              std::vector<Type>{to_type(IntType{})},
              to_type(IntType{})});
  tenv = TEnv::extend(tenv,
                      Symbol{"minus"},
                      unary_int);

  auto binary_int
      = to_type(
          ProcType{
              std::vector<Type>{to_type(IntType{}), to_type(IntType{})},
              to_type(IntType{})});
  tenv = TEnv::extend(tenv,
                      Symbol{"-"},
                      binary_int);
  tenv = TEnv::extend(tenv,
                      Symbol{"+"},
                      binary_int);
  tenv = TEnv::extend(tenv,
                      Symbol{"*"},
                      binary_int);
  tenv = TEnv::extend(tenv,
                      Symbol{"/"},
                      binary_int);

  return tenv;
}

void check_program (const std::string& text) {
  std::istringstream ss(text);
  yy::Lexer lexer(ss);
  Program result;
  yy::parser p(lexer, result);
  p.set_debug_level(getenv("YYDEBUG") != nullptr);
  p.parse();

  auto result_type = type_of(result, make_initial_tenv());
  fmt::print("result_type: {}\n", result_type);
}

void check_equal_type (const Type& actual, const Type& expected, const Expression& expression) {
  if (actual != expected) {
    report_on_unequal_type(actual, expected, expression);
  }
}

void report_on_unequal_type (const Type& actual, const Type& expected, const Expression& expression) {
  fmt::print("actual: {}, expected: {} in expression: {}", actual, expected, expression);
}

void report_on_rator_not_proc (const Type& actual, const Expression& expression) {
  fmt::print("expression {} is not a proc, but actually {}", expression, actual);
}

}