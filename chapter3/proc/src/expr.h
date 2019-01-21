//
// Created by robin on 2019-01-09.
//

#pragma once

#include <variant>
#include <boost/variant.hpp>
#include <ostream>
#include "value.h"
#include "env.h"

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
using RwOpExp = boost::recursive_wrapper<struct OpExp>;
using RwCondExp = boost::recursive_wrapper<struct CondExp>;
using RwUnpackExp = boost::recursive_wrapper<struct UnpackExp>;
using RwProcExp = boost::recursive_wrapper<struct ProcExp>;
using RwCallExp = boost::recursive_wrapper<struct CallExp>;

using Expression_ = std::variant<ConstExp,
                                 VarExp,
                                 RwOpExp,
                                 RwIfExp,
                                 RwLetExp,
                                 RwCondExp,
                                 RwUnpackExp,
                                 RwProcExp,
                                 RwCallExp>;

using Expression = std::shared_ptr<Expression_>;

std::ostream& operator << (std::ostream& os, const Expression& exp);

struct OpExp {
  Symbol rator;
  std::vector<Expression> rands;

  friend std::ostream& operator << (std::ostream& os, const OpExp& opExp);
};

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
  Symbol var;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const ProcExp& procExp);
};

struct CallExp {
  Expression rator;
  Expression rand;

  friend std::ostream& operator << (std::ostream& os, const CallExp& callExp);
};

struct Program {
  Expression exp1;

  friend std::ostream& operator << (std::ostream& os, const Program& program);
};

template<typename E>
Expression to_expr (E exp) {
  return std::make_shared<Expression_>(std::move(exp));
}

using Env = Environment<Symbol, Value>;
using SpEnv = Env::SpEnv;

struct Proc {
  Symbol var;
  Expression body;
  SpEnv saved_env;

  friend bool operator == (const Proc& lhs, const Proc& rhs) {
    return lhs.var == rhs.var &&
           lhs.body == rhs.body &&
           lhs.saved_env == rhs.saved_env;
  }
  friend bool operator != (const Proc& lhs, const Proc& rhs) {
    return !(rhs == lhs);
  }

  friend bool operator < (const Proc& lhs, const Proc& rhs) {
    if (lhs.var < rhs.var)
      return true;
    if (rhs.var < lhs.var)
      return false;
    if (lhs.body < rhs.body)
      return true;
    if (rhs.body < lhs.body)
      return false;
    return lhs.saved_env < rhs.saved_env;
  }
  friend bool operator > (const Proc& lhs, const Proc& rhs) {
    return rhs < lhs;
  }
  friend bool operator <= (const Proc& lhs, const Proc& rhs) {
    return !(rhs < lhs);
  }
  friend bool operator >= (const Proc& lhs, const Proc& rhs) {
    return !(lhs < rhs);
  }
  friend std::ostream& operator << (std::ostream& os, const Proc& proc);
};

Value proc_to_value (Proc p);
const Proc& value_to_proc (const Value& value);


}