//
// Created by robin on 2018-12-31.
//

#include "value.h"
#include <stack>
#include <iomanip>

namespace eopl {

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

  return std::visit(TypeVisitor{}, value);
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

      std::visit(*this, pair.first);

      if (auto type = type_of(pair.second); type == ValueType::PAIR) {
        os << ' ';
        std::visit(*this, pair.second);
      } else if (type == ValueType::NIL) {
        os << ')';
      } else {
        os << " . ";
        std::visit(*this, pair.second);
        os << ')';
      }
    }

    void operator () (const Array& array) {
      os << '[';
      interleave(std::begin(array),
                 std::end(array),
                 [this] (const auto& item) { std::visit(OutputVisitor{os}, item); },
                 [this] () { os << ", "; });
      os << ']';
    }
  };

  std::visit(OutputVisitor{os}, value);
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
  return std::visit(Visitor{}, value);
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
  return std::visit(Visitor{}, value);
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
  return std::visit(Visitor{}, value);
}

}