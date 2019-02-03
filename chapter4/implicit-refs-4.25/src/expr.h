//
// Created by robin on 2019-01-09.
//

#pragma once

#include "expr_fwd.h"

#include <ostream>

namespace eopl {

struct IfExp {
  Expression cond;  // cond
  Expression then_;  // then clause
  Expression else_;  // else clause

  friend std::ostream& operator << (std::ostream& os, const IfExp& exp);
};

struct BindingClause {
  Symbol var;
  Expression exp;
};

using BindingClauseList = std::vector<BindingClause>;

struct LetExp {

  BindingClauseList clauses;
  Expression body;
  bool star = false;

  friend std::ostream& operator << (std::ostream& os, const LetExp& exp);
};

struct UnpackExp {
  std::vector<Symbol> vars;
  Expression pack;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const UnpackExp& exp);
};

struct CondExp {
  struct Clause {
    Expression cond;
    Expression body;
  };
  using ClauseList = std::vector<Clause>;
  ClauseList clauses;

  friend std::ostream& operator << (std::ostream& os, const CondExp& exp);
};

struct ProcExp {
  std::vector<Symbol> params;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const ProcExp& exp);
};

struct CallExp {
  Expression rator;
  std::vector<Expression> rands;

  friend std::ostream& operator << (std::ostream& os, const CallExp& exp);
};

struct LetrecProcSpec {
  Symbol name;
  std::vector<Symbol> params;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const LetrecProcSpec& spec);
};

struct LetrecExp {
  std::vector<LetrecProcSpec> proc_list;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const LetrecExp& exp);
};

struct SequenceExp {
  std::vector<Expression> exp_list;

  friend std::ostream& operator << (std::ostream& os, const SequenceExp& sequenceExp);
};

struct AssignExp {
  Symbol var;
  Expression exp1;

  friend std::ostream& operator << (std::ostream& os, const AssignExp& assignExp);
};

struct SetdynamicExp {

  BindingClauseList clauses;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const SetdynamicExp& setdynamicExp);
};

// observers for Expression
ExpType type_of (const Expression& expression);
const ConstExp& to_const_exp (const Expression& expression);
const VarExp& to_var_exp (const Expression& expression);
const IfExp& to_if_exp (const Expression& expression);
const LetExp& to_let_exp (const Expression& expression);
const CondExp& to_cond_exp (const Expression& expression);
const UnpackExp& to_unpack_exp (const Expression& expression);
const ProcExp& to_proc_exp (const Expression& expression);
const CallExp& to_call_exp (const Expression& expression);
const LetrecExp& to_letrec_exp (const Expression& expression);
const SequenceExp& to_sequence_exp (const Expression& expression);
const AssignExp& to_assign_exp (const Expression& expression);
}