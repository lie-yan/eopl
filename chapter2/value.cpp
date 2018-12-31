//
// Created by robin on 2018-12-31.
//

#include "value.h"
#include <stack>

namespace eopl {

ValueType type_of (const Value& v) {
  struct TypeVisitor {
    ValueType operator () (Nil) { return ValueType::NIL; }
    ValueType operator () (bool) { return ValueType::BOOL; }
    ValueType operator () (int) { return ValueType::INT; }
    ValueType operator () (double) { return ValueType::DOUBLE; }
    ValueType operator () (const RwString&) { return ValueType::STRING; }
    ValueType operator () (const RwSymbol&) { return ValueType::SYMBOL; }
    ValueType operator () (const RwPair&) { return ValueType::PAIR; }
    ValueType operator () (const RwValueArray&) { return ValueType::VALUE_ARRAY; }
  };

  return std::visit(TypeVisitor{}, v);
}

std::ostream& operator << (std::ostream& os, const Value& v) {
  struct OutputVisitor {
    std::ostream& os;
    bool open_paren = false;

    void operator () (Nil) { os << "()"; }
    void operator () (bool b) { os << (b ? "#t" : "#f"); }
    void operator () (int i) { os << i; }
    void operator () (double d) { os << d; }
    void operator () (const RwString& str) { (*this)(str.get()); }
    void operator () (const RwSymbol& sym) { (*this)(sym.get()); }
    void operator () (const RwPair& pair) { (*this)(pair.get()); }
    void operator () (const RwValueArray& valueArray) { (*this)(valueArray.get()); }
    void operator () (const String& str) { os << std::quoted(str); }
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

    void operator () (const ValueArray& valueArray) {
      os << '[';
      interleave(std::begin(valueArray),
                 std::end(valueArray),
                 [this] (const auto& x) { std::visit(OutputVisitor{os}, x); },
                 [this] () { os << ", "; });
      os << ']';
    }
  };

  std::visit(OutputVisitor{os}, v);
  return os;
}
}