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
  int num;

  friend std::ostream& operator << (std::ostream& os, const ConstExp& constExp);
};

using RwDiffExp = boost::recursive_wrapper<struct DiffExp>;
using RwZeroQExp = boost::recursive_wrapper<struct ZeroQExp>;
using RwIfExp = boost::recursive_wrapper<struct IfExp>;

struct VarExp {
  Symbol var;

  friend std::ostream& operator << (std::ostream& os, const VarExp& varExp);
};

using RwLetExp = boost::recursive_wrapper<struct LetExp>;

using Expression = std::variant<ConstExp,
                                RwDiffExp,
                                RwZeroQExp,
                                RwIfExp,
                                VarExp,
                                RwLetExp>;

std::ostream& operator << (std::ostream& os, const Expression& exp);

struct DiffExp {
  Expression exp1;
  Expression exp2;

  friend std::ostream& operator << (std::ostream& os, const DiffExp& diffExp);
};

struct ZeroQExp {
  Expression exp1;

  friend std::ostream& operator << (std::ostream& os, const ZeroQExp& qExp);
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