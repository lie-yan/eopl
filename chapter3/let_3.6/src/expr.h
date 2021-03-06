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

using RwDiffExp = boost::recursive_wrapper<struct DiffExp>;
using RwMinusExp = boost::recursive_wrapper<struct MinusExp>;
using RwZeroTestExp = boost::recursive_wrapper<struct ZeroTestExp>;
using RwIfExp = boost::recursive_wrapper<struct IfExp>;
using RwLetExp = boost::recursive_wrapper<struct LetExp>;

using Expression = std::variant<ConstExp,
                                VarExp,
                                RwDiffExp,
                                RwMinusExp,
                                RwZeroTestExp,
                                RwIfExp,
                                RwLetExp>;

std::ostream& operator << (std::ostream& os, const Expression& exp);

struct DiffExp {
  Expression exp1;
  Expression exp2;

  friend std::ostream& operator << (std::ostream& os, const DiffExp& diffExp);
};

struct MinusExp {
  Expression exp1;

  friend std::ostream& operator << (std::ostream& os, const MinusExp& minusExp);
};

struct ZeroTestExp {
  Expression exp1;

  friend std::ostream& operator << (std::ostream& os, const ZeroTestExp& testExp);
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