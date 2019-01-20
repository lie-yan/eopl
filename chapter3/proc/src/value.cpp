//
// Created by robin on 2018-12-31.
//

#include "value.h"
#include <stack>
#include <iomanip>

namespace eopl {

bool operator == (const Value& lhs, const Value& rhs) {
  return *lhs == *rhs;
}

bool operator != (const Value& lhs, const Value& rhs) {
  return !(lhs == rhs);
}

ValueType type_of (const Value& value) {
  struct TypeVisitor {
    ValueType operator () (Nil) { return ValueType::NIL; }
    ValueType operator () (Bool) { return ValueType::BOOL; }
    ValueType operator () (Int) { return ValueType::INT; }
    ValueType operator () (Double) { return ValueType::DOUBLE; }
    ValueType operator () (const RwString&) { return ValueType::STRING; }
    ValueType operator () (const RwSymbol&) { return ValueType::SYMBOL; }
    ValueType operator () (const RwPair&) { return ValueType::PAIR; }
    ValueType operator () (const RwArray&) { return ValueType::ARRAY; }
    ValueType operator () (const String&) { return ValueType::STRING; }
    ValueType operator () (const Symbol&) { return ValueType::SYMBOL; }
    ValueType operator () (const Pair&) { return ValueType::PAIR; }
    ValueType operator () (const Array&) { return ValueType::ARRAY; }
  };

  return std::visit(TypeVisitor{}, *value);
}

std::ostream& operator << (std::ostream& os, const Value& value) {
  struct OutputVisitor {
    std::ostream& os;
    bool open_paren = false;

    void operator () (Nil) { os << "()"; }
    void operator () (Bool b) { os << (b.get() ? "#t" : "#f"); }
    void operator () (Int i) { os << i.get(); }
    void operator () (Double d) { os << d.get(); }
    void operator () (const RwString& str) { (*this)(str.get()); }
    void operator () (const RwSymbol& sym) { (*this)(sym.get()); }
    void operator () (const RwPair& pair) { (*this)(pair.get()); }
    void operator () (const RwArray& array) { (*this)(array.get()); }
    void operator () (const String& str) { os << std::quoted(str.get()); }
    void operator () (const Symbol& sym) { os << sym; }
    void operator () (const Pair& pair) {
      if (!open_paren) {
        open_paren = true;
        os << '(';
      }

      std::visit(OutputVisitor{os}, *pair.first);

      if (auto type = type_of(pair.second); type == ValueType::PAIR) {
        os << ' ';
        std::visit(*this, *pair.second);
      } else if (type == ValueType::NIL) {
        os << ')';
      } else {
        os << " . ";
        std::visit(*this, *pair.second);
        os << ')';
      }
    }

    void operator () (const Array& array) {
      os << '[';
      interleave(std::begin(array),
                 std::end(array),
                 [this] (const auto& item) {
                   std::visit(OutputVisitor{os}, *item);
                 },
                 [this] () { os << ", "; });
      os << ']';
    }
  };

  std::visit(OutputVisitor{os}, *value);
  return os;
}

Int value_to_int (const Value& value) {
  static const std::string msg = "unexpected type";
  struct Visitor {
    Int operator () (Nil) { throw std::runtime_error(msg); }
    Int operator () (Bool) { throw std::runtime_error(msg); }
    Int operator () (Int i) { return i; }
    Int operator () (Double) { throw std::runtime_error(msg); }
    Int operator () (const RwString&) { throw std::runtime_error(msg); }
    Int operator () (const RwSymbol&) { throw std::runtime_error(msg); }
    Int operator () (const RwPair&) { throw std::runtime_error(msg); }
    Int operator () (const RwArray&) { throw std::runtime_error(msg); }
    Int operator () (const String&) { throw std::runtime_error(msg); }
    Int operator () (const Symbol&) { throw std::runtime_error(msg); }
    Int operator () (const Pair&) { throw std::runtime_error(msg); }
    Int operator () (const Array&) { throw std::runtime_error(msg); }
  };
  return std::visit(Visitor{}, *value);
}

Bool value_to_bool (const Value& value) {
  static const std::string msg = "unexpected type";
  struct Visitor {
    Bool operator () (Nil) { throw std::runtime_error(msg); }
    Bool operator () (Bool b) { return b; }
    Bool operator () (Int) { throw std::runtime_error(msg); }
    Bool operator () (Double) { throw std::runtime_error(msg); }
    Bool operator () (const RwString&) { throw std::runtime_error(msg); }
    Bool operator () (const RwSymbol&) { throw std::runtime_error(msg); }
    Bool operator () (const RwPair&) { throw std::runtime_error(msg); }
    Bool operator () (const RwArray&) { throw std::runtime_error(msg); }
    Bool operator () (const String&) { throw std::runtime_error(msg); }
    Bool operator () (const Symbol&) { throw std::runtime_error(msg); }
    Bool operator () (const Pair&) { throw std::runtime_error(msg); }
    Bool operator () (const Array&) { throw std::runtime_error(msg); }
  };
  return std::visit(Visitor{}, *value);
}

Double value_to_double (const Value& value) {
  static const std::string msg = "unexpected type";
  struct Visitor {
    Double operator () (Nil) { throw std::runtime_error(msg); }
    Double operator () (Bool) { throw std::runtime_error(msg); }
    Double operator () (Int) { throw std::runtime_error(msg); }
    Double operator () (Double d) { return d; }
    Double operator () (const RwString&) { throw std::runtime_error(msg); }
    Double operator () (const RwSymbol&) { throw std::runtime_error(msg); }
    Double operator () (const RwPair&) { throw std::runtime_error(msg); }
    Double operator () (const RwArray&) { throw std::runtime_error(msg); }
    Double operator () (const String&) { throw std::runtime_error(msg); }
    Double operator () (const Symbol&) { throw std::runtime_error(msg); }
    Double operator () (const Pair&) { throw std::runtime_error(msg); }
    Double operator () (const Array&) { throw std::runtime_error(msg); }
  };
  return std::visit(Visitor{}, *value);
}

const String& value_to_string (const Value& value) {
  static const std::string msg = "unexpected type";
  struct Visitor {
    const String& operator () (Nil) { throw std::runtime_error(msg); }
    const String& operator () (Bool) { throw std::runtime_error(msg); }
    const String& operator () (Int) { throw std::runtime_error(msg); }
    const String& operator () (Double) { throw std::runtime_error(msg); }
    const String& operator () (const RwString& s) { return (*this)(s.get()); }
    const String& operator () (const RwSymbol&) { throw std::runtime_error(msg); }
    const String& operator () (const RwPair&) { throw std::runtime_error(msg); }
    const String& operator () (const RwArray&) { throw std::runtime_error(msg); }
    const String& operator () (const String& s) { return s; }
    const String& operator () (const Symbol&) { throw std::runtime_error(msg); }
    const String& operator () (const Pair&) { throw std::runtime_error(msg); }
    const String& operator () (const Array&) { throw std::runtime_error(msg); }
  };
  return std::visit(Visitor{}, *value);
}

const Symbol& value_to_symbol (const Value& value) {
  static const std::string msg = "unexpected type";
  struct Visitor {
    const Symbol& operator () (Nil) { throw std::runtime_error(msg); }
    const Symbol& operator () (Bool) { throw std::runtime_error(msg); }
    const Symbol& operator () (Int) { throw std::runtime_error(msg); }
    const Symbol& operator () (Double) { throw std::runtime_error(msg); }
    const Symbol& operator () (const RwString&) { throw std::runtime_error(msg); }
    const Symbol& operator () (const RwSymbol& s) { return (*this)(s.get()); }
    const Symbol& operator () (const RwPair&) { throw std::runtime_error(msg); }
    const Symbol& operator () (const RwArray&) { throw std::runtime_error(msg); }
    const Symbol& operator () (const String&) { throw std::runtime_error(msg); }
    const Symbol& operator () (const Symbol& s) { return s; }
    const Symbol& operator () (const Pair&) { throw std::runtime_error(msg); }
    const Symbol& operator () (const Array&) { throw std::runtime_error(msg); }
  };
  return std::visit(Visitor{}, *value);
}

const Pair& value_to_pair (const Value& value) {
  static const std::string msg = "unexpected type";
  struct Visitor {
    const Pair& operator () (Nil) { throw std::runtime_error(msg); }
    const Pair& operator () (Bool) { throw std::runtime_error(msg); }
    const Pair& operator () (Int) { throw std::runtime_error(msg); }
    const Pair& operator () (Double) { throw std::runtime_error(msg); }
    const Pair& operator () (const RwString&) { throw std::runtime_error(msg); }
    const Pair& operator () (const RwSymbol&) { throw std::runtime_error(msg); }
    const Pair& operator () (const RwPair& p) { return (*this)(p.get()); }
    const Pair& operator () (const RwArray&) { throw std::runtime_error(msg); }
    const Pair& operator () (const String&) { throw std::runtime_error(msg); }
    const Pair& operator () (const Symbol&) { throw std::runtime_error(msg); }
    const Pair& operator () (const Pair& p) { return p; }
    const Pair& operator () (const Array&) { throw std::runtime_error(msg); }
  };
  return std::visit(Visitor{}, *value);
}

const Array& value_to_array (const Value& value) {
  static const std::string msg = "unexpected type";
  struct Visitor {
    const Array& operator () (Nil) { throw std::runtime_error(msg); }
    const Array& operator () (Bool) { throw std::runtime_error(msg); }
    const Array& operator () (Int) { throw std::runtime_error(msg); }
    const Array& operator () (Double) { throw std::runtime_error(msg); }
    const Array& operator () (const RwString&) { throw std::runtime_error(msg); }
    const Array& operator () (const RwSymbol&) { throw std::runtime_error(msg); }
    const Array& operator () (const RwPair&) { throw std::runtime_error(msg); }
    const Array& operator () (const RwArray& a) { return (*this)(a.get()); }
    const Array& operator () (const String&) { throw std::runtime_error(msg); }
    const Array& operator () (const Symbol&) { throw std::runtime_error(msg); }
    const Array& operator () (const Pair&) { throw std::runtime_error(msg); }
    const Array& operator () (const Array& a) { return a; }
  };
  return std::visit(Visitor{}, *value);
}

Value nil_to_value (Nil n) {
  return std::make_shared<ValueCore>(n);
}

Value bool_to_value (Bool b) {
  return std::make_shared<ValueCore>(b);
}

Value int_to_value (Int i) {
  return std::make_shared<ValueCore>(i);
}

Value double_to_value (Double d) {
  return std::make_shared<ValueCore>(d);
}

Value string_to_value (String s) {
  return std::make_shared<ValueCore>(std::move(s));
}

Value symbol_to_value (Symbol s) {
  return std::make_shared<ValueCore>(std::move(s));
}

Value pair_to_value (Pair p) {
  return std::make_shared<ValueCore>(std::move(p));
}

Value array_to_value (Array a) {
  return std::make_shared<ValueCore>(std::move(a));
}

}