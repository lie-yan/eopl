//
// Created by robin on 2018-12-30.
//

#pragma once

#include "value_fwd.h"
#include "expr_fwd.h"
#include "env.h"
#include <vector>

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

struct Proc {
  const std::vector<Symbol>& params;
  Expression body;
  SpEnv saved_env;
  
  friend bool operator == (const Proc& lhs, const Proc& rhs) {
    return lhs.params == rhs.params
           && lhs.body == rhs.body
           && lhs.saved_env == rhs.saved_env;
  }
  friend bool operator != (const Proc& lhs, const Proc& rhs) { return !(rhs == lhs); }
  friend bool operator < (const Proc& lhs, const Proc& rhs) {
    if (lhs.params < rhs.params) return true;
    if (rhs.params < lhs.params) return false;
    if (lhs.body < rhs.body) return true;
    if (rhs.body < lhs.body) return false;
    return lhs.saved_env < rhs.saved_env;
  }
  friend bool operator > (const Proc& lhs, const Proc& rhs) { return rhs < lhs; }
  friend bool operator <= (const Proc& lhs, const Proc& rhs) { return !(rhs < lhs); }
  friend bool operator >= (const Proc& lhs, const Proc& rhs) { return !(lhs < rhs); }
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