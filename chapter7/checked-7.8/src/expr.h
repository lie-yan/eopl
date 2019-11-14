//
// Created by robin on 2019-01-09.
//

#pragma once

#include <ostream>
#include "expr_fwd.h"
#include "type/type_fwd.h"

namespace eopl {

struct IfExp {
  Expression cond;  // cond
  Expression then_;  // then clause
  Expression else_;  // else clause

  friend std::ostream& operator << (std::ostream& os, const IfExp& ifExp);
};

struct LetExp {
  using Clause = std::pair<Symbol, Expression>;
  using ClauseList = std::vector<Clause>;

  ClauseList clauses;
  Expression body;
  bool star = false;

  friend std::ostream& operator << (std::ostream& os, const LetExp& letExp);
};

struct UnpackExp {
  std::vector<Symbol> vars;
  Expression pack;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const UnpackExp& unpackExp);
};

struct CondExp {
  using Clause = std::pair<Expression, Expression>;
  using ClauseList = std::vector<Clause>;
  ClauseList clauses;

  friend std::ostream& operator << (std::ostream& os, const CondExp& condExp);
};

struct ParamDecl {
  Symbol sym;
  type::Type ty;

  friend bool operator == (const ParamDecl& lhs, const ParamDecl& rhs) {
    return lhs.sym == rhs.sym &&
           lhs.ty == rhs.ty;
  }
  friend bool operator != (const ParamDecl& lhs, const ParamDecl& rhs) {
    return !(rhs == lhs);
  }
  friend std::ostream& operator << (std::ostream& os, const ParamDecl& decl);
};

struct ProcExp {
  std::vector<ParamDecl> params;

  Expression body;

  friend std::ostream& operator << (std::ostream& os, const ProcExp& procExp);
};

struct CallExp {
  Expression rator;
  std::vector<Expression> rands;

  friend std::ostream& operator << (std::ostream& os, const CallExp& callExp);
};

struct LetrecProc {
  Symbol name;
  std::vector<ParamDecl> params;
  Expression body;
  type::Type result_type;

  friend std::ostream& operator << (std::ostream& os, const LetrecProc& proc);
};

struct LetrecExp {
  std::vector<LetrecProc> procs;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const LetrecExp& letrecExp);
};

struct PairExp {
  Expression first;
  Expression second;

  friend std::ostream& operator << (std::ostream& os, const PairExp& pairExp);
};

struct UnpairExp {
  std::array<Symbol, 2> vars;
  Expression exp;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const UnpairExp& unpairExp);
};

enum class ExpType {
  CONST_EXP,
  VAR_EXP,
  IF_EXP,
  LET_EXP,
  COND_EXP,
  UNPACK_EXP,
  PROC_EXP,
  CALL_EXP,
  LETREC_EXP,
  PAIR_EXP,
  UNPAIR_EXP,
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
const PairExp& to_pair_exp (const Expression& expression);
const UnpairExp& to_unpair_exp (const Expression& expression);

}