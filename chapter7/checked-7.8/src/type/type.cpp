//
// Created by Lie Yan on 2019-02-10.
//

#include "type.h"

#include "basic.h"
#include "type_fwd.h"

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
    void operator () (const RwPairType& t) { os << t.get(); }
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

bool operator == (const PairType& lhs, const PairType& rhs) {
  return lhs.first == rhs.first &&
         lhs.second == rhs.second;
}
bool operator != (const PairType& lhs, const PairType& rhs) {
  return !(rhs == lhs);
}
std::ostream& operator << (std::ostream& os, const PairType& type) {
  os << "pairof " << type.first << " * " << type.second;
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

const ProcType& to_proc_type (const Type& t) {
  return std::get<RwProcType>(*t).get();
}

const PairType& to_pair_type (const Type& t) {
  return std::get<RwPairType>(*t).get();
}

TypeEnum type_of (const Type& t) {
  struct TypeVisitor {
    TypeEnum operator () (const BoolType&) { return TypeEnum::BoolType; }
    TypeEnum operator () (const IntType&) { return TypeEnum::IntType; }
    TypeEnum operator () (const RwPairType&) { return TypeEnum::PairType; }
    TypeEnum operator () (const RwProcType&) { return TypeEnum::ProcType; }
  };

  return std::visit(TypeVisitor{}, *t);
}


}