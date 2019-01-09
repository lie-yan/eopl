//
// Created by robin on 2019-01-09.
//

#include "expr.h"

namespace eopl {

std::ostream& operator << (std::ostream& os, const Expression& exp) {
  struct OutputVisitor {
    std::ostream& os;

    void operator () (const ConstExp& e) { os << e; }
    void operator () (const DiffExp& e) { os << e; }
    void operator () (const ZeroQExp& e) { os << e; }
    void operator () (const IfExp& e) { os << e; }
    void operator () (const VarExp& e) { os << e; }
    void operator () (const LetExp& e) { os << e; }
    void operator () (const RwDiffExp& e) { (*this)(e.get()); }
    void operator () (const RwZeroQExp& e) { (*this)(e.get()); }
    void operator () (const RwIfExp& e) { (*this)(e.get()); }
    void operator () (const RwLetExp& e) { (*this)(e.get()); }
  };
  std::visit(OutputVisitor{os}, exp);
  return os;
}
std::ostream& operator << (std::ostream& os, const ConstExp& constExp) {
  os << "ConstExp(num: " << constExp.num << ")";
  return os;
}
std::ostream& operator << (std::ostream& os, const VarExp& varExp) {
  os << "VarExp(var: " << varExp.var << ")";
  return os;
}
std::ostream& operator << (std::ostream& os, const DiffExp& diffExp) {
  os << "DiffExp(exp1: " << diffExp.exp1 << ", exp2: " << diffExp.exp2 << ")";
  return os;
}
std::ostream& operator << (std::ostream& os, const ZeroQExp& qExp) {
  os << "ZeroQExp(exp1: " << qExp.exp1 << ")";
  return os;
}
std::ostream& operator << (std::ostream& os, const IfExp& ifExp) {
  os << "IfExp(exp1: " << ifExp.exp1 << ", exp2: " << ifExp.exp2 << ", exp3: " << ifExp.exp3 << ")";
  return os;
}
std::ostream& operator << (std::ostream& os, const LetExp& letExp) {
  os << "LetExp(var: " << letExp.var << ", exp1: " << letExp.exp1 << ", body: " << letExp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const Program& program) {
  os << "Program(exp1: " << program.exp1 << ")";
  return os;
}
}