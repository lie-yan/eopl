//
// Created by robin on 2018-12-31.
//

#include "value.h"

#include "expr.h"
#include "basic.h"
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
    ValueType operator () (const RwNamelessProc&) { return ValueType::NAMELESS_PROC; }
    ValueType operator () (const String&) { return ValueType::STRING; }
    ValueType operator () (const Symbol&) { return ValueType::SYMBOL; }
    ValueType operator () (const Pair&) { return ValueType::PAIR; }
    ValueType operator () (const Array&) { return ValueType::ARRAY; }
    ValueType operator () (const Proc&) { return ValueType::PROC; }
    ValueType operator () (const NamelessProc&) { return ValueType::NAMELESS_PROC; }
  };

  return std::visit(TypeVisitor{}, *value);
}

template<typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& ts) {
  interleave(std::begin(ts),
             std::end(ts),
             [&os] (const auto& x) { os << x; },
             [&os] () { os << " "; });
  return os;
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
    void operator () (const RwNamelessProc& proc) { (*this)(proc.get()); }
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
      os << "<proc " << proc.params() << ">";
    }
    void operator () (const NamelessProc& proc) {
      os << "<nameless-proc " << proc.body() << ">";
    }
  };

  std::visit(OutputVisitor{os}, *value);
  return os;
}

Int to_int (const Value& value) {
  return std::get<Int>(*value);
}

Bool to_bool (const Value& value) {
  return std::get<Bool>(*value);
}

Double to_double (const Value& value) {
  return std::get<Double>(*value);
}

const String& to_string (const Value& value) {
  return std::get<RwString>(*value).get();
}

const Symbol& to_symbol (const Value& value) {
  return std::get<RwSymbol>(*value).get();
}

const Pair& to_pair (const Value& value) {
  return std::get<RwPair>(*value).get();
}

const Array& to_array (const Value& value) {
  return std::get<RwArray>(*value).get();
}

const Proc& to_proc (const Value& value) {
  return std::get<RwProc>(*value).get();
}

Proc& to_proc (Value& value) {
  return std::get<RwProc>(*value).get();
}

const NamelessProc& to_nameless_proc (const Value& value) {
  return std::get<RwNamelessProc>(*value).get();
}

NamelessProc& to_nameless_proc (Value& value) {
  return std::get<RwNamelessProc>(*value).get();
}

std::ostream& operator << (std::ostream& os, const Proc& proc) {
  os << "Proc(params_: " << proc.params_
     << ", body: " << proc.body_
     << ", saved_env: " << proc.saved_env() << ")";
  return os;
}

Proc::Proc (const std::vector<Symbol>& params, Expression body, SpEnv saved_env)
    : params_(params), body_(std::move(body)), saved_env_(saved_env) { }

std::ostream& operator << (std::ostream& os, const NamelessProc& proc) {
  os << "NamelessProc(body: " << proc.body()
     << ", saved_env: " << proc.saved_env() << ")";
  return os;
}
SpNamelessEnv NamelessProc::saved_env () const {
  return saved_env_.lock();
}

NamelessProc::NamelessProc (Expression body, SpNamelessEnv saved_env)
    : body_(std::move(body)), saved_env_(std::move(saved_env)) { }

const Expression& NamelessProc::body () const {
  return body_;
}

void NamelessProc::saved_env (const WpNamelessEnv& saved_env) {
  saved_env_ = saved_env;
}


}