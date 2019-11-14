//
// Created by Lie Yan on 2019-02-10.
//

#pragma once

#include <ostream>
#include "type_fwd.h"
#include "expr_fwd.h"
#include "tenv.h"

namespace eopl::type {

struct PairType {
  Type first;
  Type second;

  friend bool operator == (const PairType& lhs, const PairType& rhs);
  friend bool operator != (const PairType& lhs, const PairType& rhs);

  friend std::ostream& operator << (std::ostream& os, const PairType& type);
};

struct ProcType {
  std::vector<Type> arg_types;
  Type result_type;

  friend bool operator == (const ProcType& lhs, const ProcType& rhs);
  friend bool operator != (const ProcType& lhs, const ProcType& rhs);

  friend std::ostream& operator << (std::ostream& os, const ProcType& type);
};

const ProcType& to_proc_type (const Type& t);
const PairType& to_pair_type (const Type& t);
}