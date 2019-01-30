//
// Created by Lie Yan on 2019-01-27.
//

#include "translator.h"

#include "built_in.h"
#include <fmt/ostream.h>
#include <boost/type_index.hpp>
#include <numeric>

namespace eopl {

template<typename T>
std::string error_message (const T& t) {
  return fmt::format("{} should not appear in the program for translation_of()",
                     boost::typeindex::type_id<T>().pretty_name());
}

SpStaticEnv make_initial_senv () {
  return
      StaticEnv::extend(
          StaticEnv::make_empty(),
          Symbol{"emptylist"}
      );
}

Program translation_of (const Program& program, const SpStaticEnv& senv) {
  return
      Program{
          translation_of(program.exp1, senv)
      };
}

struct TranslationOfVisitor {
  const SpStaticEnv& senv;

  template<typename T>
  Expression operator () (const boost::recursive_wrapper<T>& e) {
    return translation_of(e.get(), senv);
  }

  template<typename T>
  Expression operator () (const T& e) { return translation_of(e, senv); }
};

Expression translation_of (const Expression& exp, const SpStaticEnv& senv) {
  return std::visit(TranslationOfVisitor{senv}, *exp);
}

Expression translation_of (const ConstExp& exp, const SpStaticEnv& senv) {
  return to_exp(exp);
}

Expression translation_of (const VarExp& exp, const SpStaticEnv& senv) {
  // TODO: try to find an elegant usage of of built-in and user-defined procedures
  if (built_in::find_built_in(exp.var)) {
    return to_exp(exp);
  } else {
    return
        to_exp(
            NamelessVarExp{
                StaticEnv::apply(senv, exp.var)
            }
        );
  }
}

Expression translation_of (const NamelessVarExp& exp, const SpStaticEnv& senv) {
  throw std::runtime_error(error_message(exp));
}

Expression translation_of (const IfExp& exp, const SpStaticEnv& senv) {
  return to_exp(IfExp{
      translation_of(exp.cond, senv),
      translation_of(exp.then_, senv),
      translation_of(exp.else_, senv)
  });
}

Expression translation_of (const LetExp& exp, const SpStaticEnv& senv, std::false_type) {
  assert(!exp.star);

  std::vector<Symbol> vars;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(vars),
                 [] (const LetExp::Clause& c) {
                   return c.var;
                 });
  std::vector<Expression> exp_list;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(exp_list),
                 [] (const LetExp::Clause& c) {
                   return c.exp;
                 });
  return
      to_exp(
          NamelessLetExp{
              translation_of(exp_list, senv),
              translation_of(exp.body, StaticEnv::extend(senv, std::move(vars))),
              exp.star
          });
}

Expression translation_of (const LetExp& exp, const SpStaticEnv& senv, std::true_type) {
  assert(exp.star);

  std::vector<Expression> exp_list{};
  auto new_senv = std::accumulate(std::begin(exp.clauses), std::end(exp.clauses),
                                  senv,
                                  [&exp_list] (SpStaticEnv& p, const LetExp::Clause& c) {
                                    Expression e = translation_of(c.exp, p);
                                    exp_list.push_back(e);
                                    return StaticEnv::extend(p, c.var);
                                  });
  return
      to_exp(
          NamelessLetExp{
              exp_list,
              translation_of(exp.body, new_senv),
              exp.star
          });
}

Expression translation_of (const LetExp& exp, const SpStaticEnv& senv) {
  if (exp.star) {
    return translation_of(exp, senv, std::true_type());
  } else {
    return translation_of(exp, senv, std::false_type());
  }
}

Expression translation_of (const NamelessLetExp& exp, const SpStaticEnv& senv) {
  throw std::runtime_error(error_message(exp));
}

Expression translation_of (const CondExp& exp, const SpStaticEnv& senv) {
  CondExp::ClauseList target;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(target),
                 [&senv] (const CondExp::Clause& c) {
                   return CondExp::Clause{
                       translation_of(c.cond, senv),
                       translation_of(c.body, senv)};
                 });

  return
      to_exp(
          CondExp{
              std::move(target)
          });
}

Expression translation_of (const UnpackExp& exp, const SpStaticEnv& senv) {
  Expression pack = translation_of(exp.pack, senv);
  Expression body = translation_of(exp.body, StaticEnv::extend(senv, exp.vars));

  return
      to_exp(
          NamelessUnpackExp{
              exp.vars.size(),
              pack,
              body
          });
}

Expression translation_of (const NamelessUnpackExp& exp, const SpStaticEnv& senv) {
  throw std::runtime_error(error_message(exp));
}

Expression translation_of (const ProcExp& exp, const SpStaticEnv& senv) {
  return
      to_exp(
          NamelessProcExp{
              translation_of(exp.body, StaticEnv::extend(senv, exp.params))
          });
}

Expression translation_of (const NamelessProcExp& exp, const SpStaticEnv& senv) {
  throw std::runtime_error(error_message(exp));
}

Expression translation_of (const CallExp& exp, const SpStaticEnv& senv) {
  return
      to_exp(
          CallExp{
              translation_of(exp.rator, senv),
              translation_of(exp.rands, senv)
          });
}

Expression translation_of (const LetrecExp& exp, const SpStaticEnv& senv) {
  std::vector<Symbol> proc_names;
  std::transform(std::begin(exp.proc_list), std::end(exp.proc_list),
                 std::back_inserter(proc_names),
                 [] (const LetrecProcSpec& spec) {
                   return spec.name;
                 });
  auto letrec_senv = StaticEnv::extend(senv, std::move(proc_names));

  auto to_nameless = [&letrec_senv] (const LetrecProcSpec& spec) {
    auto new_senv = StaticEnv::extend(letrec_senv, spec.params);

    return NamelessLetrecProcSpec{
        spec.params.size(),
        translation_of(spec.body, new_senv)
    };
  };

  std::vector<NamelessLetrecProcSpec> proc_list;
  std::transform(std::begin(exp.proc_list), std::end(exp.proc_list),
                 std::back_inserter(proc_list),
                 to_nameless);
  auto body = translation_of(exp.body, letrec_senv);

  return to_exp(
      NamelessLetrecExp{
          std::move(proc_list),
          std::move(body)
      });
}

Expression translation_of (const NamelessLetrecExp& exp, const SpStaticEnv& senv) {
  throw std::runtime_error(error_message(exp));
}

std::vector<Expression> translation_of (const std::vector<Expression>& exp_list, const SpStaticEnv& senv) {
  std::vector<Expression> ret;
  std::transform(std::begin(exp_list),
                 std::end(exp_list),
                 std::back_inserter(ret),
                 [&senv] (const Expression& e) -> Expression {
                   return translation_of(e, senv);
                 });
  return ret;
}

}