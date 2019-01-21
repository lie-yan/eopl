//
// Created by robin on 2019-01-09.
//

#include "expr.h"

namespace eopl {

std::ostream& operator << (std::ostream& os, const Expression& exp) {
  struct OutputVisitor {
    std::ostream& os;

    void operator () (const ConstExp& e) { os << e; }
    void operator () (const IfExp& e) { os << e; }
    void operator () (const VarExp& e) { os << e; }
    void operator () (const LetExp& e) { os << e; }
    void operator () (const CondExp& e) { os << e; }
    void operator () (const UnpackExp& e) { os << e; }
    void operator () (const ProcExp& e) { os << e; }
    void operator () (const CallExp& e) { os << e; }
    void operator () (const RwIfExp& e) { (*this)(e.get()); }
    void operator () (const RwLetExp& e) { (*this)(e.get()); }
    void operator () (const RwCondExp& e) { (*this)(e.get()); }
    void operator () (const RwUnpackExp& e) { (*this)(e.get()); }
    void operator () (const RwProcExp& e) { (*this)(e.get()); }
    void operator () (const RwCallExp& e) { (*this)(e.get()); }
  };
  std::visit(OutputVisitor{os}, *exp);
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

std::ostream& operator << (std::ostream& os, const IfExp& ifExp) {
  os << "IfExp(" << ifExp.cond << ", " << ifExp.then_ << ", " << ifExp.else_ << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const Program& program) {
  os << "Program(" << program.exp1 << ")";
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
  os << "(" << clause.first << " ==> " << clause.second << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const LetExp::Clause& clause) {
  os << "(" << clause.first << " = " << clause.second << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const CondExp& condExp) {
  os << "CondExp(" << condExp.clauses << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const LetExp& letExp) {
  os << "LetExp(clauses: " << letExp.clauses << ", body: " << letExp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const UnpackExp& unpackExp) {
  os << "UnpackExp(vars: " << unpackExp.vars
     << ", pack: " << unpackExp.pack
     << ", body: " << unpackExp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const ProcExp& procExp) {
  os << "ProcExp(vars: " << procExp.vars << ", body: " << procExp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const CallExp& callExp) {
  os << "CallExp(rator: " << callExp.rator
     << ", rands: " << callExp.rands
     << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const Proc& proc) {
  os << "Proc(vars: " << proc.vars
     << ", body: " << proc.body
     << ", saved_env: " << proc.saved_env << ")";
  return os;
}

ExprType type_of (const Expression& expression) {
  struct TypeVisitor {
    ExprType operator () (const ConstExp&) { return ExprType::CONST_EXP; }
    ExprType operator () (const VarExp&) { return ExprType::VAR_EXP; }
    ExprType operator () (const IfExp&) { return ExprType::IF_EXP; }
    ExprType operator () (const CondExp&) { return ExprType::COND_EXP; }
    ExprType operator () (const LetExp&) { return ExprType::LET_EXP; }
    ExprType operator () (const UnpackExp&) { return ExprType::UNPACK_EXP; }
    ExprType operator () (const ProcExp&) { return ExprType::PROC_EXP; }
    ExprType operator () (const CallExp&) { return ExprType::CALL_EXP; }
    ExprType operator () (const RwIfExp&) { return ExprType::IF_EXP; }
    ExprType operator () (const RwCondExp&) { return ExprType::COND_EXP; }
    ExprType operator () (const RwLetExp&) { return ExprType::LET_EXP; }
    ExprType operator () (const RwUnpackExp&) { return ExprType::UNPACK_EXP; }
    ExprType operator () (const RwProcExp&) { return ExprType::PROC_EXP; }
    ExprType operator () (const RwCallExp&) { return ExprType::CALL_EXP; }
  };

  return std::visit(TypeVisitor{}, *expression);
}

const VarExp& to_var_exp (const Expression& expression) {
  return std::get<VarExp>(*expression);
}


}