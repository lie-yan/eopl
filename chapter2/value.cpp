//
// Created by robin on 2018-12-31.
//

#include "value.h"
#include <stack>

namespace eopl {

ValueType type_of (const Value& v) {
  struct TypeVisitor {
    ValueType operator () (Nil) { return ValueType::NIL; }
    ValueType operator () (bool b) { return ValueType::BOOL; }
    ValueType operator () (int i) { return ValueType::INT; }
    ValueType operator () (double d) { return ValueType::DOUBLE; }
    ValueType operator () (const RwString& str) { return ValueType::STRING; }
    ValueType operator () (const RwSymbol& sym) { return ValueType::SYMBOL; }
    ValueType operator () (const RwPair& p) { return ValueType::PAIR; }
    ValueType operator () (const RwValueList& d) { return ValueType::VALUE_LIST; }
  };

  return std::visit(TypeVisitor{}, v);
}

std::ostream& operator << (std::ostream& os, const Value& v) {
  struct OutputVisitor {
    std::ostream& os;
    bool open_paren = false;

    void operator () (Nil) { os << "nil"; }
    void operator () (bool b) { os << std::boolalpha << b; }
    void operator () (int i) { os << i; }
    void operator () (double d) { os << d; }
    void operator () (const RwString& str) { os << std::quoted(str.get()); }
    void operator () (const RwSymbol& sym) { os << sym.get(); }
    void operator () (const RwPair& p) {
      if (!open_paren) {
        os << '(';
        open_paren = true;
      }

      std::visit(*this, p.get().first);

      if (auto type = type_of(p.get().second); type == ValueType::PAIR) {
        os << ' ';
        std::visit(*this, p.get().second);
      } else if (type == ValueType::NIL) {
        os << ')';
      } else {
        os << " . ";
        std::visit(*this, p.get().second);
        os << ')';
      }
    }

    void operator () (const RwValueList& d) {
      os << "[";
      interleave(std::begin(d.get()),
                 std::end(d.get()),
                 [this] (const auto& x) {
                   std::visit(OutputVisitor{os}, x);
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