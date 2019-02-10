//
// Created by Lie Yan on 2019-02-10.
//

#pragma once

#include <ostream>
#include "type_fwd.h"
#include "expr_fwd.h"
#include "tenv.h"

namespace eopl::type {

struct ProcType {
  std::vector<Type> arg_types;
  Type result_type;

  friend bool operator == (const ProcType& lhs, const ProcType& rhs);
  friend bool operator != (const ProcType& lhs, const ProcType& rhs);

  friend std::ostream& operator << (std::ostream& os, const ProcType& type);
};


}