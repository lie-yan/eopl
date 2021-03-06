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

struct LetExp {
  struct Clause {
    Symbol var;
    Expression exp;
  };
  using ClauseList = std::vector<Clause>;

  ClauseList clauses;
  Expression body;
  bool star = false;
  bool mutable_ = false;

  friend std::ostream& operator << (std::ostream& os, const LetExp& exp);
};

//struct NamelessLetExp {
//  std::vector<Expression> clauses;
//  Expression body;
//  bool star = false;
//
//  friend std::ostream& operator << (std::ostream& os, const NamelessLetExp& exp);
//};

struct UnpackExp {
  std::vector<Symbol> vars;
  Expression pack;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const UnpackExp& exp);
};

//struct NamelessUnpackExp {
//  size_t var_num;
//  Expression pack;
//  Expression body;
//
//  friend std::ostream& operator << (std::ostream& os, const NamelessUnpackExp& exp);
//};

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

//struct NamelessProcExp {
//  Expression body;
//
//  friend std::ostream& operator << (std::ostream& os, const NamelessProcExp& exp);
//};

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

//struct NamelessLetrecProcSpec {
//  size_t param_num;
//  Expression body;
//
//  friend std::ostream& operator << (std::ostream& os, const NamelessLetrecProcSpec& spec);
//};

//struct NamelessLetrecExp {
//  std::vector<NamelessLetrecProcSpec> procs;
//  Expression body;
//
//  friend std::ostream& operator << (std::ostream& os, const NamelessLetrecExp& exp);
//};

struct SequenceExp {
  std::vector<Expression> exp_list;

  friend std::ostream& operator << (std::ostream& os, const SequenceExp& sequenceExp);
};

struct AssignExp {
  Symbol var;
  Expression exp1;

  friend std::ostream& operator << (std::ostream& os, const AssignExp& assignExp);
};

struct Program {
  Expression exp1;

  friend std::ostream& operator << (std::ostream& os, const Program& program);
};

// observers for Expression
ExpType type_of (const Expression& expression);
const ConstExp& to_const_exp (const Expression& expression);
const VarExp& to_var_exp (const Expression& expression);
//const NamelessVarExp& to_nameless_var_exp (const Expression& expression);
const IfExp& to_if_exp (const Expression& expression);
const LetExp& to_let_exp (const Expression& expression);
//const NamelessLetExp& to_nameless_let_exp (const Expression& expression);
const CondExp& to_cond_exp (const Expression& expression);
const UnpackExp& to_unpack_exp (const Expression& expression);
//const NamelessUnpackExp& to_nameless_unpack_exp (const Expression& expression);
const ProcExp& to_proc_exp (const Expression& expression);
//const NamelessProcExp& to_nameless_proc_exp (const Expression& expression);
const CallExp& to_call_exp (const Expression& expression);
const LetrecExp& to_letrec_exp (const Expression& expression);
//const NamelessLetrecExp& to_nameless_letrec_exp (const Expression& expression);
const SequenceExp& to_sequence_exp (const Expression& expression);
const AssignExp& to_assign_exp (const Expression& expression);
}