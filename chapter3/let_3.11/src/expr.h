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

using Expression = std::variant<ConstExp,
                                VarExp,
                                RwOpExp,
                                RwIfExp,
                                RwLetExp>;

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
  Symbol var;
  Expression exp1;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const LetExp& letExp);
};

struct Program {
  Expression exp1;

  friend std::ostream& operator << (std::ostream& os, const Program& program);
};

}