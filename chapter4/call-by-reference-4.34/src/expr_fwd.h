//
// Created by Lie Yan on 2019-01-26.
//

#pragma once

#include "value_fwd.h"

#include <ostream>

namespace eopl {

enum class ExpType {
  CONST_EXP,
  VAR_EXP,
  IF_EXP,
  LET_EXP,
  COND_EXP,
  UNPACK_EXP,
  PROC_EXP,
  SUBR_EXP,
  CALL_EXP,
  LETREC_EXP,
  SEQUENCE_EXP,
  ASSIGN_EXP,
  SETDYNAMIC_EXP,
};

struct ConstExp {
  Int num;

  friend std::ostream& operator << (std::ostream& os, const ConstExp& constExp);
};

struct VarExp {
  Symbol var;

  friend std::ostream& operator << (std::ostream& os, const VarExp& varExp);
};

using RwIfExp = boost::recursive_wrapper<struct IfExp>;
using RwLetExp = boost::recursive_wrapper<struct LetExp>;
using RwCondExp = boost::recursive_wrapper<struct CondExp>;
using RwUnpackExp = boost::recursive_wrapper<struct UnpackExp>;
using RwProcExp = boost::recursive_wrapper<struct ProcExp>;
using RwSubrExp = boost::recursive_wrapper<struct SubrExp>;
using RwCallExp = boost::recursive_wrapper<struct CallExp>;
using RwLetrecExp = boost::recursive_wrapper<struct LetrecExp>;
using RwSequenceExp = boost::recursive_wrapper<struct SequenceExp>;
using RwAssignExp = boost::recursive_wrapper<struct AssignExp>;
using RwSetdynamicExp = boost::recursive_wrapper<struct SetdynamicExp>;

using Expression_ = std::variant<ConstExp,
                                 VarExp,
                                 RwIfExp,
                                 RwLetExp,
                                 RwCondExp,
                                 RwUnpackExp,
                                 RwProcExp,
                                 RwSubrExp,
                                 RwCallExp,
                                 RwLetrecExp,
                                 RwSequenceExp,
                                 RwAssignExp,
                                 RwSetdynamicExp>;

using Expression = std::shared_ptr<Expression_>;
std::ostream& operator << (std::ostream& os, const Expression& exp);

// constructors for Expression
template<typename T>
Expression to_exp (T&& exp) {
  return std::make_shared<Expression_>(std::forward<T>(exp));
}

struct Program;

}