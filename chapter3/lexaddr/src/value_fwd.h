//
// Created by Lie Yan on 2019-01-26.
//

#pragma once

#include <boost/variant.hpp>
#include <ostream>

namespace eopl {

enum class ValueType {
  NIL,
  BOOL,
  INT,
  DOUBLE,
  STRING,
  SYMBOL,
  PAIR,
  ARRAY,
  PROC,
  NAMELESS_PROC,
};

struct Nil {
  friend bool operator == (const Nil& lhs, const Nil& rhs) {
    return true;
  }
  friend bool operator != (const Nil& lhs, const Nil& rhs) {
    return !(rhs == lhs);
  }
  friend std::ostream& operator << (std::ostream& os, const Nil& nil) {
    os << "()";
    return os;
  }
};

class String {
public:
  String () = default;
  explicit String (std::string val) : val(std::move(val)) { }
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
  friend bool operator < (const String& lhs, const String& rhs) {
    return lhs.val < rhs.val;
  }
  friend bool operator > (const String& lhs, const String& rhs) {
    return rhs < lhs;
  }
  friend bool operator <= (const String& lhs, const String& rhs) {
    return !(rhs < lhs);
  }
  friend bool operator >= (const String& lhs, const String& rhs) {
    return !(lhs < rhs);
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
  Symbol () = default;
  explicit Symbol (std::string val) : val(std::move(val)) { }
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
  friend bool operator < (const Symbol& lhs, const Symbol& rhs) {
    return lhs.val < rhs.val;
  }
  friend bool operator > (const Symbol& lhs, const Symbol& rhs) {
    return rhs < lhs;
  }
  friend bool operator <= (const Symbol& lhs, const Symbol& rhs) {
    return !(rhs < lhs);
  }
  friend bool operator >= (const Symbol& lhs, const Symbol& rhs) {
    return !(lhs < rhs);
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
  friend bool operator < (const Bool& lhs, const Bool& rhs) {
    return lhs.val < rhs.val;
  }
  friend bool operator > (const Bool& lhs, const Bool& rhs) {
    return rhs < lhs;
  }
  friend bool operator <= (const Bool& lhs, const Bool& rhs) {
    return !(rhs < lhs);
  }
  friend bool operator >= (const Bool& lhs, const Bool& rhs) {
    return !(lhs < rhs);
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
  friend bool operator < (const Int& lhs, const Int& rhs) {
    return lhs.val < rhs.val;
  }
  friend bool operator > (const Int& lhs, const Int& rhs) {
    return rhs < lhs;
  }
  friend bool operator <= (const Int& lhs, const Int& rhs) {
    return !(rhs < lhs);
  }
  friend bool operator >= (const Int& lhs, const Int& rhs) {
    return !(lhs < rhs);
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
  friend bool operator < (const Double& lhs, const Double& rhs) {
    return lhs.val < rhs.val;
  }
  friend bool operator > (const Double& lhs, const Double& rhs) {
    return rhs < lhs;
  }
  friend bool operator <= (const Double& lhs, const Double& rhs) {
    return !(rhs < lhs);
  }
  friend bool operator >= (const Double& lhs, const Double& rhs) {
    return !(lhs < rhs);
  }
  friend std::ostream& operator << (std::ostream& os, const Double& aDouble) {
    os << aDouble.val;
    return os;
  }

private:
  double val;
};

using RwString = boost::recursive_wrapper<struct String>;
using RwSymbol = boost::recursive_wrapper<struct Symbol>;
using RwArray = boost::recursive_wrapper<struct Array>;
using RwPair = boost::recursive_wrapper<struct Pair>;
using RwProc = boost::recursive_wrapper<struct Proc>;
using RwNamelessProc = boost::recursive_wrapper<struct NamelessProc>;

template<typename T>
bool operator == (const boost::recursive_wrapper<T>& lhs,
                  const boost::recursive_wrapper<T>& rhs) {
  return lhs.get() == rhs.get();
}

template<typename T>
bool operator != (const boost::recursive_wrapper<T>& lhs,
                  const boost::recursive_wrapper<T>& rhs) {
  return !(rhs == lhs);
}

using Value_ = std::variant<Nil,
                            Bool,
                            Int,
                            Double,
                            RwString,
                            RwSymbol,
                            RwPair,
                            RwArray,
                            RwProc,
                            RwNamelessProc>;

using Value = std::shared_ptr<Value_>;

bool operator == (const Value& lhs, const Value& rhs);
bool operator != (const Value& lhs, const Value& rhs);
std::ostream& operator << (std::ostream& os, const Value& value);

/// constructors for `Value` below
template<typename T>
Value to_value (T&& v) { return std::make_shared<Value_>(std::forward<T>(v)); }

}