//
// Created by robin on 2019-01-09.
//

#include "expr.h"

#include "basic.h"

namespace eopl {

struct ExpressionOutputVisitor {
  std::ostream& os;

  template<typename T>
  void operator () (const boost::recursive_wrapper<T>& exp) { os << exp.get(); }

  template<typename T>
  void operator () (const T& exp) { os << exp; }
};

std::ostream& operator << (std::ostream& os, const Expression& exp) {
  std::visit(ExpressionOutputVisitor{os}, *exp);
  return os;
}

std::ostream& operator << (std::ostream& os, const ConstExp& constExp) {
  os << "ConstExp(" << constExp.num << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const VarExp& varExp) {
  os << "VarExp(" << varExp.var << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const IfExp& exp) {
  os << "IfExp(" << exp.cond << ", " << exp.then_ << ", " << exp.else_ << ")";
  return os;
}

template<typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& ts) {
  interleave(std::begin(ts),
             std::end(ts),
             [&os] (const auto& x) { os << x; },
             [&os] () { os << ", "; });
  return os;
}

std::ostream& operator << (std::ostream& os, const CondExp::Clause& clause) {
  os << "(" << clause.cond << " ==> " << clause.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const CondExp& exp) {
  os << "CondExp(" << exp.clauses << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const LetExp& exp) {
  os << "LetExp(clauses: " << exp.clauses << ", body: " << exp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const UnpackExp& exp) {
  os << "UnpackExp(vars: " << exp.vars
     << ", pack: " << exp.pack
     << ", body: " << exp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const ProcExp& exp) {
  os << "ProcExp(params: " << exp.params
     << ", body: " << exp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const SubrExp& subrExp) {
  os << "SubrExp(params: " << subrExp.params
     << ", body: " << subrExp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const CallExp& exp) {
  os << "CallExp(rator: " << exp.rator
     << ", rands: " << exp.rands
     << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const LetrecProcSpec& spec) {
  os << "LetrecProcSpec(name: " << spec.name
     << ", params: " << spec.params
     << ", body: " << spec.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const LetrecExp& exp) {
  os << "LetrecExp(proc_list: " << exp.proc_list
     << ", body: " << exp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const SequenceExp& sequenceExp) {
  os << "SequenceExp(" << sequenceExp.exp_list << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const AssignExp& assignExp) {
  os << "AssignExp(var: " << assignExp.var
     << ", exp1: " << assignExp.exp1 << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const BindingClause& clause) {
  os << "(" << clause.var << " = " << clause.exp << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const SetdynamicExp& setdynamicExp) {
  os << "SetdynamicExp(clauses: " << setdynamicExp.clauses
     << ", body: " << setdynamicExp.body << ")";
  return os;
}

ExpType type_of (const Expression& expression) {
  struct TypeVisitor {
    ExpType operator () (const ConstExp&) { return ExpType::CONST_EXP; }
    ExpType operator () (const VarExp&) { return ExpType::VAR_EXP; }
    ExpType operator () (const RwIfExp&) { return ExpType::IF_EXP; }
    ExpType operator () (const RwCondExp&) { return ExpType::COND_EXP; }
    ExpType operator () (const RwLetExp&) { return ExpType::LET_EXP; }
    ExpType operator () (const RwUnpackExp&) { return ExpType::UNPACK_EXP; }
    ExpType operator () (const RwProcExp&) { return ExpType::PROC_EXP; }
    ExpType operator () (const RwSubrExp&) { return ExpType::SUBR_EXP; }
    ExpType operator () (const RwCallExp&) { return ExpType::CALL_EXP; }
    ExpType operator () (const RwLetrecExp&) { return ExpType::LETREC_EXP; }
    ExpType operator () (const RwSequenceExp&) { return ExpType::SEQUENCE_EXP; }
    ExpType operator () (const RwAssignExp&) { return ExpType::ASSIGN_EXP; }
    ExpType operator () (const RwSetdynamicExp&) { return ExpType::SETDYNAMIC_EXP; }
  };

  return std::visit(TypeVisitor{}, *expression);
}

const ConstExp& to_const_exp (const Expression& expression) {
  return std::get<ConstExp>(*expression);
}

const VarExp& to_var_exp (const Expression& expression) {
  return std::get<VarExp>(*expression);
}

const IfExp& to_if_exp (const Expression& expression) {
  return std::get<RwIfExp>(*expression).get();
}

const LetExp& to_let_exp (const Expression& expression) {
  return std::get<RwLetExp>(*expression).get();
}

const CondExp& to_cond_exp (const Expression& expression) {
  return std::get<RwCondExp>(*expression).get();
}

const UnpackExp& to_unpack_exp (const Expression& expression) {
  return std::get<RwUnpackExp>(*expression).get();
}

const ProcExp& to_proc_exp (const Expression& expression) {
  return std::get<RwProcExp>(*expression).get();
}

const SubrExp& to_subr_exp (const Expression& expression) {
  return std::get<RwSubrExp>(*expression).get();
}

const CallExp& to_call_exp (const Expression& expression) {
  return std::get<RwCallExp>(*expression).get();
}

const LetrecExp& to_letrec_exp (const Expression& expression) {
  return std::get<RwLetrecExp>(*expression).get();
}

const SequenceExp& to_sequence_exp (const Expression& expression) {
  return std::get<RwSequenceExp>(*expression).get();
}

const AssignExp& to_assign_exp (const Expression& expression) {
  return std::get<RwAssignExp>(*expression).get();
}


}