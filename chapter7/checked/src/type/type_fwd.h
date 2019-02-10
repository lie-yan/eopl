//
// Created by Lie Yan on 2019-02-10.
//

#pragma once

#include <ostream>
#include <boost/variant.hpp>

namespace eopl::type {

enum class TypeEnum {
  BoolType,
  IntType,
  ProcType,
};

struct BoolType {

  friend bool operator == (const BoolType&, const BoolType&) { return true; }
  friend bool operator != (const BoolType&, const BoolType&) { return false; }

  friend std::ostream& operator << (std::ostream& os, const BoolType& type) {
    os << "BoolType";
    return os;
  }
};

struct IntType {
  friend bool operator == (const IntType&, const IntType&) { return true; }
  friend bool operator != (const IntType&, const IntType&) { return false; }

  friend std::ostream& operator << (std::ostream& os, const IntType& type) {
    os << "IntType";
    return os;
  }
};

using RwProcType = boost::recursive_wrapper<struct ProcType>;

template<typename T>
bool operator == (const boost::recursive_wrapper<T>& lhs, const boost::recursive_wrapper<T>& rhs) {
  return lhs.get() == rhs.get();
}

template<typename T>
bool operator != (const boost::recursive_wrapper<T>& lhs, const boost::recursive_wrapper<T>& rhs) {
  return !(rhs == lhs);
}

using Type_ = std::variant<BoolType,
                           IntType,
                           RwProcType>;

using Type = std::shared_ptr<Type_>;

bool operator == (const Type& lhs, const Type& rhs);
bool operator != (const Type& lhs, const Type& rhs);
std::ostream& operator << (std::ostream& os, const Type&);

template<typename T>
Type to_type (T&& t) { return std::make_shared<Type_>(std::forward<T>(t)); }

TypeEnum type_of (const Type& t);

}