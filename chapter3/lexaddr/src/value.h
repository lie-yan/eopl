//
// Created by robin on 2018-12-30.
//

#pragma once

#include "expr_fwd.h"
#include "value_fwd.h"
#include "env.h"
#include "nameless/nenv.h"

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

struct Proc {
  const std::vector<Symbol>& params;
  Expression body;
  SpEnv saved_env;

  friend bool operator == (const Proc& lhs, const Proc& rhs) {
    return lhs.params == rhs.params &&
           lhs.body == rhs.body &&
           lhs.saved_env == rhs.saved_env;
  }
  friend bool operator != (const Proc& lhs, const Proc& rhs) {
    return !(rhs == lhs);
  }
  friend std::ostream& operator << (std::ostream& os, const Proc& proc);
};

struct NamelessProc {
  Expression body;
  SpNamelessEnv saved_env;

  friend bool operator == (const NamelessProc& lhs, const NamelessProc& rhs) {
    return lhs.body == rhs.body &&
           lhs.saved_env == rhs.saved_env;
  }
  friend bool operator != (const NamelessProc& lhs, const NamelessProc& rhs) {
    return !(rhs == lhs);
  }

  friend std::ostream& operator << (std::ostream& os, const NamelessProc& proc);
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
const NamelessProc& to_nameless_proc (const Value& value);

}