//
// Created by robin on 2018-12-30.
//

#pragma once

#include <string>
#include <variant>
#include <vector>
#include <iomanip>
#include <boost/variant.hpp>
#include "basic.h"

namespace eopl {

using RwString = boost::recursive_wrapper<struct String>;
using RwSymbol = boost::recursive_wrapper<struct Symbol>;
using RwValueList = boost::recursive_wrapper<struct ValueList>;

using Value = std::variant<std::monostate,
                           bool,
                           int,
                           double,
                           RwString,
                           RwSymbol,
                           RwValueList>;

struct String : std::string { using std::string::string; };

struct Symbol : std::string { using std::string::string; };

struct ValueList : std::vector<Value> { using std::vector<Value>::vector; };

std::ostream& operator << (std::ostream& os, const Value& v) {
  struct OutputVisitor {
    std::ostream& os;

    void operator () (std::monostate) { os << "(uninitialized value)"; }
    void operator () (bool b) { os << std::boolalpha << b; }
    void operator () (int i) { os << i; }
    void operator () (double d) { os << d; }
    void operator () (const RwString& s) { os << std::quoted(s.get()); }
    void operator () (const RwSymbol& s) { os << s.get(); }
    void operator () (const RwValueList& d) {
      os << "[";
      interleave(std::begin(d.get()),
                 std::end(d.get()),
                 [this] (const auto& x) {
                   std::visit(*this, x);
                 },
                 [this] () {
                   os << ", ";
                 });
      os << "]";
    }
  };

  std::visit(OutputVisitor{os}, v);
  return os;
}

}