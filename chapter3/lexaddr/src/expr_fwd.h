//
// Created by Lie Yan on 2019-01-26.
//

#pragma once

#include "value_fwd.h"
#include "nameless/senv.h"

#include <ostream>

namespace eopl {

enum class ExpType {
  CONST_EXP,
  VAR_EXP,
  NAMELESS_VAR_EXP,
  IF_EXP,
  LET_EXP,
  NAMELESS_LET_EXP,
  COND_EXP,
  UNPACK_EXP,
  NAMELESS_UNPACK_EXP,
  PROC_EXP,
  NAMELESS_PROC_EXP,
  CALL_EXP,
  LETREC_EXP,
};

struct ConstExp {
  Int num;

  friend std::ostream& operator << (std::ostream& os, const ConstExp& constExp);
};

struct VarExp {
  Symbol var;

  friend std::ostream& operator << (std::ostream& os, const VarExp& varExp);
};

struct NamelessVarExp {
  LexicalAddr lexicalAddr;

  friend std::ostream& operator << (std::ostream& os, const NamelessVarExp& varExp);
};

using RwIfExp = boost::recursive_wrapper<struct IfExp>;
using RwLetExp = boost::recursive_wrapper<struct LetExp>;
using RwNamelessLetExp = boost::recursive_wrapper<struct NamelessLetExp>;
using RwCondExp = boost::recursive_wrapper<struct CondExp>;
using RwUnpackExp = boost::recursive_wrapper<struct UnpackExp>;
using RwNamelessUnpackExp = boost::recursive_wrapper<struct NamelessUnpackExp>;
using RwProcExp = boost::recursive_wrapper<struct ProcExp>;
using RwNamelessProcExp = boost::recursive_wrapper<struct NamelessProcExp>;
using RwCallExp = boost::recursive_wrapper<struct CallExp>;
using RwLetrecExp = boost::recursive_wrapper<struct LetrecExp>;

using Expression_ = std::variant<ConstExp,
                                 VarExp,
                                 NamelessVarExp,
                                 RwIfExp,
                                 RwLetExp,
                                 RwNamelessLetExp,
                                 RwCondExp,
                                 RwUnpackExp,
                                 RwNamelessUnpackExp,
                                 RwProcExp,
                                 RwNamelessProcExp,
                                 RwCallExp,
                                 RwLetrecExp>;

using Expression = std::shared_ptr<Expression_>;
std::ostream& operator << (std::ostream& os, const Expression& exp);

// constructors for Expression
template<typename T>
Expression to_exp (T&& exp) {
  return std::make_shared<Expression_>(std::forward<T>(exp));
}

struct Program;

}