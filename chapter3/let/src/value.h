//
// Created by robin on 2018-12-30.
//

#pragma once

#include <string>
#include <variant>
#include <vector>
#include <boost/variant.hpp>
#include <ostream>
#include "basic.h"

namespace eopl {

using RwString = boost::recursive_wrapper<struct String>;
using RwSymbol = boost::recursive_wrapper<struct Symbol>;
using RwArray = boost::recursive_wrapper<struct Array>;
using RwPair = boost::recursive_wrapper<struct Pair>;

struct Nil { };
struct String : std::string { using std::string::string; };
struct Symbol : std::string { using std::string::string; };

struct Bool {
  bool val;

  Bool () : Bool(false) { }
  Bool (bool val) : val(val) { }

  friend
  std::ostream& operator << (std::ostream& os, const Bool& aBool) {
    os << std::boolalpha << aBool.val;
    return os;
  }
};

struct Int {
  int val;

  Int () : Int(0) { }
  Int (int val) : val(val) { }

  friend
  std::ostream& operator << (std::ostream& os, const Int& anInt) {
    os << anInt.val;
    return os;
  }
};

struct Double {
  double val;

  Double () : Double(0) { }
  Double (double val) : val(val) { }

  friend
  std::ostream& operator << (std::ostream& os, const Double& aDouble) {
    os << aDouble.val;
    return os;
  }
};

using Value = std::variant<Nil,
                           Bool,
                           Int,
                           Double,
                           RwString,
                           RwSymbol,
                           RwPair,
                           RwArray>;

struct Pair {
  Value first;
  Value second;
};

struct Array : std::vector<Value> { using std::vector<Value>::vector; };

enum class ValueType {
  NIL,
  BOOL,
  INT,
  DOUBLE,
  STRING,
  SYMBOL,
  PAIR,
  ARRAY,
};

ValueType type_of (const Value& value);
Int value_to_int (const Value& value);
Bool value_to_bool (const Value& value);
Double value_to_double (const Value& value);

std::ostream& operator << (std::ostream& os, const Value& value);

}