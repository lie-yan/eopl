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
    void operator () (const OpExp& e) { os << e; }
    void operator () (const CondExp& e) { os << e; }
    void operator () (const RwIfExp& e) { (*this)(e.get()); }
    void operator () (const RwLetExp& e) { (*this)(e.get()); }
    void operator () (const RwOpExp& e) { (*this)(e.get()); }
    void operator () (const RwCondExp& e) { (*this)(e.get()); }
  };
  std::visit(OutputVisitor{os}, exp);
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
  os << "IfExp(" << ifExp.exp1 << ", " << ifExp.exp2 << ", " << ifExp.exp3 << ")";
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

std::ostream& operator << (std::ostream& os, const OpExp& opExp) {
  os << "OpExp(rator: " << opExp.rator << ", rands: " << opExp.rands << ")";
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
}