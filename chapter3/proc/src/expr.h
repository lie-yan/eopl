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
using RwOpExp = boost::recursive_wrapper<struct OpExp>;
using RwCondExp = boost::recursive_wrapper<struct CondExp>;
using RwUnpackExp = boost::recursive_wrapper<struct UnpackExp>;
using RwProcExp = boost::recursive_wrapper<struct ProcExp>;
using RwCallExp = boost::recursive_wrapper<struct CallExp>;

using Expression = std::variant<ConstExp,
                                VarExp,
                                RwOpExp,
                                RwIfExp,
                                RwLetExp,
                                RwCondExp,
                                RwUnpackExp,
                                RwProcExp,
                                RwCallExp>;

std::ostream& operator << (std::ostream& os, const Expression& exp);

struct OpExp {
  Symbol rator;
  std::vector<Expression> rands;

  friend std::ostream& operator << (std::ostream& os, const OpExp& opExp);
};

struct IfExp {
  Expression exp1;  // cond
  Expression exp2;  // then clause
  Expression exp3;  // else clause

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

}