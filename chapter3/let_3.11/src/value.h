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

template<typename T>
bool operator == (const boost::recursive_wrapper<T>& lhs, const boost::recursive_wrapper<T>& rhs) {
  return lhs.get() == rhs.get();
}

template<typename T>
bool operator != (const boost::recursive_wrapper<T>& lhs, const boost::recursive_wrapper<T>& rhs) {
  return !(rhs == lhs);
}

struct Nil {
  friend bool operator == (const Nil& lhs, const Nil& rhs) {
    return true;
  }
  friend bool operator != (const Nil& lhs, const Nil& rhs) {
    return !(rhs == lhs);
  }
};

class String {
public:
  String () { }
  String (std::string val) : val(std::move(val)) { }
  String (const String&) = default;
  String (String&&) = default;
  String& operator = (const String&) = default;
  String& operator = (String&&) = default;

  const std::string& get () const { return val; }

  friend bool operator == (const String& lhs, const String& rhs) {
    return lhs.val == rhs.val;
  }
  friend bool operator != (const String& lhs, const String& rhs) {
    return !(rhs == lhs);
  }
  friend std::ostream& operator << (std::ostream& os, const String& string) {
    os << string.val;
    return os;
  }

private:
  std::string val;
};

class Symbol {
public:
  Symbol () { }
  Symbol (std::string val) : val(std::move(val)) { }
  Symbol (const Symbol&) = default;
  Symbol (Symbol&&) = default;
  Symbol& operator = (const Symbol&) = default;
  Symbol& operator = (Symbol&&) = default;

  const std::string& get () const { return val; }

  friend bool operator == (const Symbol& lhs, const Symbol& rhs) {
    return lhs.val == rhs.val;
  }
  friend bool operator != (const Symbol& lhs, const Symbol& rhs) {
    return !(rhs == lhs);
  }
  friend std::ostream& operator << (std::ostream& os, const Symbol& string) {
    os << string.val;
    return os;
  }

private:
  std::string val;
};

class Bool {
public:
  Bool () : Bool(false) { }
  explicit Bool (bool val) : val(val) { }

  bool get () const { return val; }

  friend bool operator == (const Bool& lhs, const Bool& rhs) {
    return lhs.val == rhs.val;
  }
  friend bool operator != (const Bool& lhs, const Bool& rhs) {
    return !(rhs == lhs);
  }

  friend std::ostream& operator << (std::ostream& os, const Bool& aBool) {
    os << std::boolalpha << aBool.val;
    return os;
  }

private:
  bool val;
};

class Int {
public:
  Int () : Int(0) { }
  explicit Int (int val) : val(val) { }

  int get () const { return val; }

  friend bool operator == (const Int& lhs, const Int& rhs) {
    return lhs.val == rhs.val;
  }
  friend bool operator != (const Int& lhs, const Int& rhs) {
    return !(rhs == lhs);
  }

  friend std::ostream& operator << (std::ostream& os, const Int& anInt) {
    os << anInt.val;
    return os;
  }
private:
  int val;
};

class Double {
public:
  Double () : Double(0) { }
  explicit Double (double val) : val(val) { }

  double get () const { return val; }

  friend bool operator == (const Double& lhs, const Double& rhs) {
    return lhs.val == rhs.val;
  }
  friend bool operator != (const Double& lhs, const Double& rhs) {
    return !(rhs == lhs);
  }
  friend std::ostream& operator << (std::ostream& os, const Double& aDouble) {
    os << aDouble.val;
    return os;
  }
private:
  double val;
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

  friend bool operator == (const Pair& lhs, const Pair& rhs) {
    return lhs.first == rhs.first &&
           lhs.second == rhs.second;
  }
  friend bool operator != (const Pair& lhs, const Pair& rhs) {
    return !(rhs == lhs);
  }
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