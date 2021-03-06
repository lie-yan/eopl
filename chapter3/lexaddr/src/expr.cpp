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

std::ostream& operator << (std::ostream& os, const NamelessVarExp& exp) {
  os << "NamelessVarExp(" << exp.addr << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const IfExp& exp) {
  os << "IfExp(" << exp.cond << ", " << exp.then_ << ", " << exp.else_ << ")";
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
  os << "(" << clause.cond << " ==> " << clause.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const CondExp& exp) {
  os << "CondExp(" << exp.clauses << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const LetExp::Clause& clause) {
  os << "(" << clause.var << " = " << clause.exp << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const LetExp& exp) {
  os << "LetExp(clauses: " << exp.clauses << ", body: " << exp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const NamelessLetExp& exp) {
  os << "NamelessLetExp(clauses: " << exp.clauses
     << ", body: " << exp.body
     << ", star: " << std::boolalpha << exp.star << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const UnpackExp& exp) {
  os << "UnpackExp(vars: " << exp.vars
     << ", pack: " << exp.pack
     << ", body: " << exp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const NamelessUnpackExp& exp) {
  os << "NamelessUnpackExp(var_num: " << exp.var_num
     << ", pack: " << exp.pack
     << ", body: " << exp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const ProcExp& exp) {
  os << "ProcExp(params: " << exp.params
     << ", body: " << exp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const NamelessProcExp& exp) {
  os << "NamelessProcExp(" << exp.body << ")";
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

std::ostream& operator << (std::ostream& os, const NamelessLetrecProcSpec& spec) {
  os << "NamelessLetrecProcSpec(param_num: " << spec.param_num
     << ", body: " << spec.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const NamelessLetrecExp& exp) {
  os << "NamelessLetrecExp(procs: " << exp.procs
     << ", body: " << exp.body << ")";
  return os;
}

ExpType type_of (const Expression& expression) {
  struct TypeVisitor {
    ExpType operator () (const ConstExp&) { return ExpType::CONST_EXP; }
    ExpType operator () (const VarExp&) { return ExpType::VAR_EXP; }
    ExpType operator () (const NamelessVarExp&) { return ExpType::NAMELESS_VAR_EXP; }
    ExpType operator () (const RwIfExp&) { return ExpType::IF_EXP; }
    ExpType operator () (const RwCondExp&) { return ExpType::COND_EXP; }
    ExpType operator () (const RwLetExp&) { return ExpType::LET_EXP; }
    ExpType operator () (const RwNamelessLetExp&) { return ExpType::NAMELESS_LET_EXP; }
    ExpType operator () (const RwUnpackExp&) { return ExpType::UNPACK_EXP; }
    ExpType operator () (const RwNamelessUnpackExp&) { return ExpType::NAMELESS_UNPACK_EXP; }
    ExpType operator () (const RwProcExp&) { return ExpType::PROC_EXP; }
    ExpType operator () (const RwNamelessProcExp&) { return ExpType::NAMELESS_PROC_EXP; }
    ExpType operator () (const RwCallExp&) { return ExpType::CALL_EXP; }
    ExpType operator () (const RwLetrecExp&) { return ExpType::LETREC_EXP; }
    ExpType operator () (const RwNamelessLetrecExp&) { return ExpType::NAMELESS_LETREC_EXP; }
  };

  return std::visit(TypeVisitor{}, *expression);
}

const ConstExp& to_const_exp (const Expression& expression) {
  return std::get<ConstExp>(*expression);
}

const VarExp& to_var_exp (const Expression& expression) {
  return std::get<VarExp>(*expression);
}

const NamelessVarExp& to_nameless_var_exp (const Expression& expression) {
  return std::get<NamelessVarExp>(*expression);
}

const IfExp& to_if_exp (const Expression& expression) {
  return std::get<RwIfExp>(*expression).get();
}

const LetExp& to_let_exp (const Expression& expression) {
  return std::get<RwLetExp>(*expression).get();
}

const NamelessLetExp& to_nameless_let_exp (const Expression& expression) {
  return std::get<RwNamelessLetExp>(*expression).get();
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

const NamelessProcExp& to_nameless_proc_exp (const Expression& expression) {
  return std::get<RwNamelessProcExp>(*expression).get();
}

const CallExp& to_call_exp (const Expression& expression) {
  return std::get<RwCallExp>(*expression).get();
}

const LetrecExp& to_letrec_exp (const Expression& expression) {
  return std::get<RwLetrecExp>(*expression).get();
}

const NamelessUnpackExp& to_nameless_unpack_exp (const Expression& expression) {
  return std::get<RwNamelessUnpackExp>(*expression).get();
}

const NamelessLetrecExp& to_nameless_letrec_exp (const Expression& expression) {
  return std::get<RwNamelessLetrecExp>(*expression).get();
}

}