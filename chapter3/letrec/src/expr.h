//
// Created by robin on 2019-01-09.
//

#pragma once

#include <variant>
#include <boost/variant.hpp>
#include <ostream>
#include "value.h"

namespace eopl {

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
using RwCallExp = boost::recursive_wrapper<struct CallExp>;
using RwLetrecExp = boost::recursive_wrapper<struct LetrecExp>;

using Expression_ = std::variant<ConstExp,
                                 VarExp,
                                 RwIfExp,
                                 RwLetExp,
                                 RwCondExp,
                                 RwUnpackExp,
                                 RwProcExp,
                                 RwCallExp,
                                 RwLetrecExp>;

using Expression = std::shared_ptr<Expression_>;
std::ostream& operator << (std::ostream& os, const Expression& exp);

struct IfExp {
  Expression cond;  // cond
  Expression then_;  // then clause
  Expression else_;  // else clause

  friend std::ostream& operator << (std::ostream& os, const IfExp& ifExp);
};

struct LetExp {
  using Clause = std::pair<Symbol, Expression>;
  using ClauseList = std::vector<Clause>;

  ClauseList clauses;
  Expression body;
  bool star = false;

  friend std::ostream& operator << (std::ostream& os, const LetExp& letExp);
};

struct UnpackExp {
  std::vector<Symbol> vars;
  Expression pack;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const UnpackExp& unpackExp);
};

struct CondExp {
  using Clause = std::pair<Expression, Expression>;
  using ClauseList = std::vector<Clause>;
  ClauseList clauses;

  friend std::ostream& operator << (std::ostream& os, const CondExp& condExp);
};

struct ProcExp {
  std::vector<Symbol> params;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const ProcExp& procExp);
};

struct CallExp {
  Expression rator;
  std::vector<Expression> rands;

  friend std::ostream& operator << (std::ostream& os, const CallExp& callExp);
};

struct LetrecProc {
  Symbol name;
  std::vector<Symbol> params;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const LetrecProc& def);
};

struct LetrecExp {
  std::vector<LetrecProc> procs;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const LetrecExp& letrecExp);
};

struct Program {
  Expression exp1;

  friend std::ostream& operator << (std::ostream& os, const Program& program);
};

//using Env = Environment<Symbol, Value>;
//using SpEnv = Env::SpEnv;
//
//struct Proc {
//  const std::vector<Symbol>& params;
//  Expression body;
//  SpEnv saved_env;
//
//  friend bool operator == (const Proc& lhs, const Proc& rhs) {
//    return lhs.params == rhs.params &&
//           lhs.body == rhs.body &&
//           lhs.saved_env == rhs.saved_env;
//  }
//  friend bool operator != (const Proc& lhs, const Proc& rhs) {
//    return !(rhs == lhs);
//  }
//  friend bool operator < (const Proc& lhs, const Proc& rhs) {
//    if (lhs.params < rhs.params)
//      return true;
//    if (rhs.params < lhs.params)
//      return false;
//    if (lhs.body < rhs.body)
//      return true;
//    if (rhs.body < lhs.body)
//      return false;
//    return lhs.saved_env < rhs.saved_env;
//  }
//  friend bool operator > (const Proc& lhs, const Proc& rhs) {
//    return rhs < lhs;
//  }
//  friend bool operator <= (const Proc& lhs, const Proc& rhs) {
//    return !(rhs < lhs);
//  }
//  friend bool operator >= (const Proc& lhs, const Proc& rhs) {
//    return !(lhs < rhs);
//  }
//  friend std::ostream& operator << (std::ostream& os, const Proc& proc);
//};

enum class ExpType {
  CONST_EXP,
  VAR_EXP,
  IF_EXP,
  LET_EXP,
  COND_EXP,
  UNPACK_EXP,
  PROC_EXP,
  CALL_EXP,
  LETREC_EXP,
};

// constructors for Expression
template<typename T>
Expression to_exp (T&& exp) {
  return std::make_shared<Expression_>(std::forward<T>(exp));
}

// observers for Expression
ExpType type_of (const Expression& expression);
const ConstExp& to_const_exp (const Expression& expression);
const VarExp& to_var_exp (const Expression& expression);
const IfExp& to_if_exp (const Expression& expression);
const LetExp& to_let_exp (const Expression& expression);
const CondExp& to_cond_exp (const Expression& expression);
const UnpackExp& to_unpack_exp (const Expression& expression);
const ProcExp& to_proc_exp (const Expression& expression);
const CallExp& to_call_exp (const Expression& expression);
const LetrecExp& to_letrec_exp (const Expression& expression);


}