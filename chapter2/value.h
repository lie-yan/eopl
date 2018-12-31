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
using RwValueArray = boost::recursive_wrapper<struct ValueArray>;
using RwPair = boost::recursive_wrapper<struct Pair>;

struct Nil { };
struct String : std::string { using std::string::string; };
struct Symbol : std::string { using std::string::string; };

using Value = std::variant<Nil,
                           bool,
                           int,
                           double,
                           RwString,
                           RwSymbol,
                           RwPair,
                           RwValueArray>;

struct Pair {
  Value first;
  Value second;
};

struct ValueArray : std::vector<Value> { using std::vector<Value>::vector; };

enum class ValueType {
  NIL,
  BOOL,
  INT,
  DOUBLE,
  STRING,
  SYMBOL,
  PAIR,
  VALUE_ARRAY,
};

ValueType type_of (const Value& v);

std::ostream& operator << (std::ostream& os, const Value& v);

}