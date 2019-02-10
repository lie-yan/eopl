//
// Created by Lie Yan on 2019-02-10.
//

#include "type.h"

#include "basic.h"

namespace eopl::type {

bool operator == (const Type& lhs, const Type& rhs) {
  return *lhs == *rhs;
}

bool operator != (const Type& lhs, const Type& rhs) {
  return !(lhs == rhs);
}

std::ostream& operator << (std::ostream& os, const Type& t) {
  struct OutputVisitor {
    std::ostream& os;

    void operator () (const BoolType& t) { os << t; }
    void operator () (const IntType& t) { os << t; }
    void operator () (const RwProcType& t) { os << t.get(); }
  };

  std::visit(OutputVisitor{os}, *t);
  return os;
}

std::ostream& operator << (std::ostream& os, const std::vector<Type>& types) {
  interleave(std::begin(types), std::end(types),
             [&os] (const Type& t) { os << t; },
             [&os] () { os << ", "; });
  return os;
}

std::ostream& operator << (std::ostream& os, const ProcType& type) {
  os << "(" << type.arg_types << " -> " << type.result_type << ")";
  return os;
}

bool operator == (const ProcType& lhs, const ProcType& rhs) {
  return lhs.arg_types == rhs.arg_types &&
         lhs.result_type == rhs.result_type;
}

bool operator != (const ProcType& lhs, const ProcType& rhs) {
  return !(rhs == lhs);
}

}