//
// Created by robin on 2018-12-31.
//

#include "value.h"
#include "expr.h"
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
    ValueType operator () (const RwProc&) { return ValueType::PROC; }
    ValueType operator () (const String&) { return ValueType::STRING; }
    ValueType operator () (const Symbol&) { return ValueType::SYMBOL; }
    ValueType operator () (const Pair&) { return ValueType::PAIR; }
    ValueType operator () (const Array&) { return ValueType::ARRAY; }
    ValueType operator () (const Proc&) { return ValueType::PROC; }
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
    void operator () (const RwProc& proc) { (*this)(proc.get()); }
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
    void operator () (const Proc& proc) {
      os << "<proc " << proc.var << ">";
    }
  };

  std::visit(OutputVisitor{os}, *value);
  return os;
}

Int value_to_int (const Value& value) {
  return std::get<Int>(*value);
}

Bool value_to_bool (const Value& value) {
  return std::get<Bool>(*value);
}

Double value_to_double (const Value& value) {
  return std::get<Double>(*value);
}

const String& value_to_string (const Value& value) {
  return std::get<RwString>(*value).get();
}

const Symbol& value_to_symbol (const Value& value) {
  return std::get<RwSymbol>(*value).get();
}

const Pair& value_to_pair (const Value& value) {
  return std::get<RwPair>(*value).get();
}

const Array& value_to_array (const Value& value) {
  return std::get<RwArray>(*value).get();
}

const Proc& value_to_proc (const Value& value) {
  return std::get<RwProc>(*value).get();
}

Value nil_to_value (Nil n) {
  return std::make_shared<Value_>(n);
}

Value bool_to_value (Bool b) {
  return std::make_shared<Value_>(b);
}

Value int_to_value (Int i) {
  return std::make_shared<Value_>(i);
}

Value double_to_value (Double d) {
  return std::make_shared<Value_>(d);
}

Value string_to_value (String s) {
  return std::make_shared<Value_>(std::move(s));
}

Value symbol_to_value (Symbol s) {
  return std::make_shared<Value_>(std::move(s));
}

Value pair_to_value (Pair p) {
  return std::make_shared<Value_>(std::move(p));
}

Value array_to_value (Array a) {
  return std::make_shared<Value_>(std::move(a));
}

Value proc_to_value (Proc p) {
  return std::make_shared<Value_>(std::move(p));
}

}