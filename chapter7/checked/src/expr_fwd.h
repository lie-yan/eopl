//
// Created by Lie Yan on 2019-02-10.
//

#pragma once

#include "value_fwd.h"

#include <variant>
#include <boost/variant.hpp>
#include <ostream>

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

// constructors for Expression
template<typename T>
Expression to_exp (T&& exp) {
  return std::make_shared<Expression_>(std::forward<T>(exp));
}


}