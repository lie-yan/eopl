//
// Created by robin on 2019-01-09.
//

#include "expr.h"

#include "value.h"

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
    void operator () (const LetrecExp& e) { os << e; }
    void operator () (const RwIfExp& e) { (*this)(e.get()); }
    void operator () (const RwLetExp& e) { (*this)(e.get()); }
    void operator () (const RwCondExp& e) { (*this)(e.get()); }
    void operator () (const RwUnpackExp& e) { (*this)(e.get()); }
    void operator () (const RwProcExp& e) { (*this)(e.get()); }
    void operator () (const RwCallExp& e) { (*this)(e.get()); }
    void operator () (const RwLetrecExp& e) { (*this)(e.get()); }
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
  os << "ProcExp(params: " << procExp.params
     << ", body: " << procExp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const CallExp& callExp) {
  os << "CallExp(rator: " << callExp.rator
     << ", rands: " << callExp.rands
     << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const Proc& proc) {
  os << "Proc(params: " << proc.params
     << ", body: " << proc.body
     << ", saved_env: " << proc.saved_env << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const LetrecProc& def) {
  os << "LetrecProc(name: " << def.name
     << ", params: " << def.params
     << ", body: " << def.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const LetrecExp& letrecExp) {
  os << "LetrecExp(procs: " << letrecExp.procs
     << ", body: " << letrecExp.body << ")";
  return os;
}

ExpType type_of (const Expression& expression) {
  struct TypeVisitor {
    ExpType operator () (const ConstExp&) { return ExpType::CONST_EXP; }
    ExpType operator () (const VarExp&) { return ExpType::VAR_EXP; }
    ExpType operator () (const IfExp&) { return ExpType::IF_EXP; }
    ExpType operator () (const CondExp&) { return ExpType::COND_EXP; }
    ExpType operator () (const LetExp&) { return ExpType::LET_EXP; }
    ExpType operator () (const UnpackExp&) { return ExpType::UNPACK_EXP; }
    ExpType operator () (const ProcExp&) { return ExpType::PROC_EXP; }
    ExpType operator () (const CallExp&) { return ExpType::CALL_EXP; }
    ExpType operator () (const LetrecExp&) { return ExpType::LETREC_EXP; }
    ExpType operator () (const RwIfExp&) { return ExpType::IF_EXP; }
    ExpType operator () (const RwCondExp&) { return ExpType::COND_EXP; }
    ExpType operator () (const RwLetExp&) { return ExpType::LET_EXP; }
    ExpType operator () (const RwUnpackExp&) { return ExpType::UNPACK_EXP; }
    ExpType operator () (const RwProcExp&) { return ExpType::PROC_EXP; }
    ExpType operator () (const RwCallExp&) { return ExpType::CALL_EXP; }
    ExpType operator () (const RwLetrecExp&) { return ExpType::LETREC_EXP; }
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

const CallExp& to_call_exp (const Expression& expression) {
  return std::get<RwCallExp>(*expression).get();
}

const LetrecExp& to_letrec_exp (const Expression& expression) {
  return std::get<RwLetrecExp>(*expression).get();
}

}