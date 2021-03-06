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
    void operator () (const ZeroTestExp& e) { os << e; }
    void operator () (const IfExp& e) { os << e; }
    void operator () (const VarExp& e) { os << e; }
    void operator () (const LetExp& e) { os << e; }
    void operator () (const RwDiffExp& e) { (*this)(e.get()); }
    void operator () (const RwZeroTestExp& e) { (*this)(e.get()); }
    void operator () (const RwIfExp& e) { (*this)(e.get()); }
    void operator () (const RwLetExp& e) { (*this)(e.get()); }
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
std::ostream& operator << (std::ostream& os, const DiffExp& diffExp) {
  os << "DiffExp(" << diffExp.exp1 << ", " << diffExp.exp2 << ")";
  return os;
}
std::ostream& operator << (std::ostream& os, const ZeroTestExp& qExp) {
  os << "ZeroTestExp(" << qExp.exp1 << ")";
  return os;
}
std::ostream& operator << (std::ostream& os, const IfExp& ifExp) {
  os << "IfExp(" << ifExp.exp1 << ", " << ifExp.exp2 << ", " << ifExp.exp3 << ")";
  return os;
}
std::ostream& operator << (std::ostream& os, const LetExp& letExp) {
  os << "LetExp(" << letExp.var << ", " << letExp.exp1 << ", " << letExp.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const Program& program) {
  os << "Program(" << program.exp1 << ")";
  return os;
}
}