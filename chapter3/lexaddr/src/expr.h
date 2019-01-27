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

struct NamelessLetExp {
  std::vector<Expression> clauses;
  Expression body;
  bool star = false;

  friend std::ostream& operator << (std::ostream& os, const NamelessLetExp& letExp);
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

struct ProcExp {
  std::vector<Symbol> params;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const ProcExp& procExp);
};

struct NamelessProcExp {
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const NamelessProcExp& procExp);
};

struct CallExp {
  Expression rator;
  std::vector<Expression> rands;

  friend std::ostream& operator << (std::ostream& os, const CallExp& callExp);
};

struct LetrecProcSpec {
  Symbol name;
  std::vector<Symbol> params;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const LetrecProcSpec& def);
};

struct LetrecExp {
  std::vector<LetrecProcSpec> procs;
  Expression body;

  friend std::ostream& operator << (std::ostream& os, const LetrecExp& letrecExp);
};

struct Program {
  Expression exp1;

  friend std::ostream& operator << (std::ostream& os, const Program& program);
};

// observers for Expression
ExpType type_of (const Expression& expression);
const ConstExp& to_const_exp (const Expression& expression);
const VarExp& to_var_exp (const Expression& expression);
const NamelessVarExp& to_nameless_var_exp (const Expression& expression);
const IfExp& to_if_exp (const Expression& expression);
const LetExp& to_let_exp (const Expression& expression);
const NamelessLetExp& to_nameless_let_exp (const Expression& expression);
const CondExp& to_cond_exp (const Expression& expression);
const UnpackExp& to_unpack_exp (const Expression& expression);
const ProcExp& to_proc_exp (const Expression& expression);
const NamelessProcExp& to_nameless_proc_exp (const Expression& expression);
const CallExp& to_call_exp (const Expression& expression);
const LetrecExp& to_letrec_exp (const Expression& expression);

}