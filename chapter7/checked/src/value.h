//
// Created by robin on 2018-12-30.
//

#pragma once

#include "value_fwd.h"
#include "expr_fwd.h"
#include "type/type_fwd.h"
#include "env.h"
#include <vector>
#include <ostream>

namespace eopl {

struct Pair {
  Value first;
  Value second;

  friend bool operator == (const Pair& lhs, const Pair& rhs) {
    return lhs.first == rhs.first &&
           lhs.second == rhs.second;
  }
  friend bool operator != (const Pair& lhs, const Pair& rhs) {
    return !(rhs == lhs);
  }
};

struct Array : std::vector<Value> {
  using std::vector<Value>::vector;
};

struct ParamDecl;

struct Proc {
  std::vector<ParamDecl> params;
  Expression body;
  SpEnv saved_env;

  friend bool operator == (const Proc& lhs, const Proc& rhs);
  friend bool operator != (const Proc& lhs, const Proc& rhs);

  friend std::ostream& operator << (std::ostream& os, const Proc& proc);
};

/// observers for `Value` below
ValueType type_of (const Value& value);
Int to_int (const Value& value);
Bool to_bool (const Value& value);
Double to_double (const Value& value);
const String& to_string (const Value& value);
const Symbol& to_symbol (const Value& value);
const Pair& to_pair (const Value& value);
const Array& to_array (const Value& value);
const Proc& to_proc (const Value& value);
Proc& to_proc (Value& value);

}