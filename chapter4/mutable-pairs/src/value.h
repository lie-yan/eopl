//
// Created by robin on 2018-12-30.
//

#pragma once

#include "value_fwd.h"
#include "expr_fwd.h"
#include "stmt_fwd.h"
#include "env.h"
#include "store.h"

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

class Proc {
public:
  Proc (const std::vector<Symbol>& params, Expression body, SpEnv saved_env);

  friend bool operator == (const Proc& lhs, const Proc& rhs) {
    return lhs.params_ == rhs.params_ &&
           lhs.body_ == rhs.body_ &&
           lhs.saved_env() == rhs.saved_env();
  }
  friend bool operator != (const Proc& lhs, const Proc& rhs) {
    return !(rhs == lhs);
  }

  friend std::ostream& operator << (std::ostream& os, const Proc& proc);

  const std::vector<Symbol>& params () const { return params_; }
  const Expression& body () const { return body_; }
  SpEnv saved_env () const { return saved_env_; }
  void saved_env (const SpEnv& env) { saved_env_ = env; }
private:
  std::vector<Symbol> params_;
  Expression body_;
  SpEnv saved_env_;
};

class Subr {
public:
  Subr (const std::vector<Symbol>& params, Statement body, SpEnv saved_env);

  friend bool operator == (const Subr& lhs, const Subr& rhs) {
    return lhs.params_ == rhs.params_ &&
           lhs.body_ == rhs.body_ &&
           lhs.saved_env_ == rhs.saved_env_;
  }
  friend bool operator != (const Subr& lhs, const Subr& rhs) {
    return !(rhs == lhs);
  }

  const std::vector<Symbol>& params () const { return params_; }
  const Statement& body () const { return body_; }
  SpEnv saved_env () const { return saved_env_; }
  void saved_env (const SpEnv& env) { saved_env_ = env; }
private:
  const std::vector<Symbol>& params_;
  Statement body_;
  SpEnv saved_env_;
};

class MutPair {
public:
  MutPair (Ref left_ref,  SpStore store);

  friend bool operator == (const MutPair& lhs, const MutPair& rhs) {
    return lhs.left() == rhs.left() &&
           lhs.right() == rhs.right();
  }
  friend bool operator != (const MutPair& lhs, const MutPair& rhs) {
    return !(rhs == lhs);
  }

  Value left () const;
  Value right () const;

  void left(Value value);
  void right(Value value);

  Ref left_ref () const;
  Ref right_ref () const;

private:
  Ref left_ref_;
  SpStore store_;
};

/// observers for `Value` below
ValueType type_of (const Value& value);
Int to_int (const Value& value);
Bool to_bool (const Value& value);
Double to_double (const Value& value);
const String& to_string (const Value& value);
const Symbol& to_symbol (const Value& value);
const Pair& to_pair (const Value& value);
const MutPair& to_mut_pair (const Value& value);
const Array& to_array (const Value& value);
const Proc& to_proc (const Value& value);
Proc& to_proc (Value& value);
const Subr& to_subr (const Value& value);
Subr& to_subr (Value& value);
const Ref& to_ref (const Value& value);
Ref& to_ref (Value& value);

std::optional<std::vector<Value>> flatten (Value lst);

}