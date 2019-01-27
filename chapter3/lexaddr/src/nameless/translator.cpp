//
// Created by Lie Yan on 2019-01-27.
//

#include <built_in.h>
#include "translator.h"

namespace eopl {

SpStaticEnv make_initial_senv () {
  auto ret = StaticEnv::make_empty();
  ret = StaticEnv::extend(ret, Symbol{"emptylist"});
  return ret;
}

Program translation_of (const Program& program, const SpStaticEnv& senv) {
  return Program{translation_of(program.exp1, senv)};
}

struct TranslationOfVisitor {
  const SpStaticEnv& senv;

  template<typename T>
  Expression operator () (const boost::recursive_wrapper<T>& e) { return translation_of(e.get(), senv); }

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
  if (built_in::find_built_in(exp.var)) {
    return to_exp(exp);
  } else {
    return to_exp(NamelessVarExp{StaticEnv::apply(senv, exp.var)});
  }
}

Expression translation_of (const NamelessVarExp& exp, const SpStaticEnv& senv) {
  throw std::runtime_error("NamelessVarExp should not appear in the expression for translation_of()");
}

Expression translation_of (const IfExp& exp, const SpStaticEnv& senv) {

  return to_exp(IfExp{
      translation_of(exp.cond, senv),
      translation_of(exp.then_, senv),
      translation_of(exp.else_, senv)
  });
}

Expression translation_of (const LetExp& exp, const SpStaticEnv& senv) {

  std::vector<Symbol> vars;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(vars),
                 [] (const LetExp::Clause& c) {
                   return c.first;
                 });
  std::vector<Expression> exps;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(exps),
                 [] (const LetExp::Clause& c) {
                   return c.second;
                 });
  return to_exp(NamelessLetExp{
      translation_of(exps, senv),
      translation_of(exp.body, StaticEnv::extend(senv, std::move(vars))),
      exp.star
  });
}

Expression translation_of (const NamelessLetExp& exp, const SpStaticEnv& senv) {
  throw std::runtime_error("NamelessLetExp should not appear in the expression for translation_of()");
}

Expression translation_of (const CondExp& exp, const SpStaticEnv& senv) {
  CondExp::ClauseList target;
  std::transform(std::begin(exp.clauses), std::end(exp.clauses),
                 std::back_inserter(target),
                 [&senv] (const CondExp::Clause& c) {
                   return CondExp::Clause{
                       translation_of(c.first, senv),
                       translation_of(c.second, senv)};
                 });

  return to_exp(CondExp{
      std::move(target)
  });
}

Expression translation_of (const UnpackExp& exp, const SpStaticEnv& senv) {
  Expression pack = translation_of(exp.pack, senv);
  Expression body = translation_of(exp.body, StaticEnv::extend(senv, exp.vars));

  return to_exp(NamelessUnpackExp{exp.vars.size(), pack, body});
}

Expression translation_of (const NamelessUnpackExp& exp, const SpStaticEnv& senv) {
  throw std::runtime_error("NamelessUnpackExp should not appear here");
}

Expression translation_of (const ProcExp& exp, const SpStaticEnv& senv) {
  return to_exp(NamelessProcExp{
      translation_of(exp.body, StaticEnv::extend(senv, exp.params))
  });
}

Expression translation_of (const NamelessProcExp& exp, const SpStaticEnv& senv) {
  throw std::runtime_error("NamelessProcExp should not appear here");
}

Expression translation_of (const CallExp& exp, const SpStaticEnv& senv) {
  return to_exp(CallExp{
      translation_of(exp.rator, senv),
      translation_of(exp.rands, senv)
  });
}

Expression translation_of (const LetrecExp& exp, const SpStaticEnv& senv) {
  std::vector<Symbol> names;
  std::transform(std::begin(exp.procs), std::end(exp.procs),
                 std::back_inserter(names),
                 [] (const LetrecProcSpec& spec) {
                   return spec.name;
                 });
  auto letrec_senv = StaticEnv::extend(senv, std::move(names));

  auto to_nameless = [&letrec_senv] (const LetrecProcSpec& spec) {
    auto new_senv = StaticEnv::extend(letrec_senv, spec.params);

    return NamelessLetrecProcSpec{
        spec.params.size(),
        translation_of(spec.body, new_senv)
    };
  };

  std::vector<NamelessLetrecProcSpec> procs;
  std::transform(std::begin(exp.procs), std::end(exp.procs),
                 std::back_inserter(procs),
                 to_nameless);
  auto body = translation_of(exp.body, letrec_senv);

  return to_exp(NamelessLetrecExp{std::move(procs), std::move(body)});
}

Expression translation_of (const NamelessLetrecExp& exp, const SpStaticEnv& senv) {
  throw std::runtime_error("NamelessLetrecExp should not appear here");
}

std::vector<Expression> translation_of (const std::vector<Expression>& exps, const SpStaticEnv& senv) {
  std::vector<Expression> target;

  std::transform(std::begin(exps), std::end(exps),
                 std::back_inserter(target),
                 [&senv] (const Expression& e) {
                   return translation_of(e, senv);
                 });
  return target;
}


}