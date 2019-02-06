//
// Created by robin on 2018-12-31.
//

#include "value.h"

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
    ValueType operator () (Unit) { return ValueType::UNIT; }
    ValueType operator () (Nil) { return ValueType::NIL; }
    ValueType operator () (Bool) { return ValueType::BOOL; }
    ValueType operator () (Int) { return ValueType::INT; }
    ValueType operator () (Double) { return ValueType::DOUBLE; }
    ValueType operator () (const RwString&) { return ValueType::STRING; }
    ValueType operator () (const RwSymbol&) { return ValueType::SYMBOL; }
    ValueType operator () (const RwPair&) { return ValueType::PAIR; }
    ValueType operator () (const RwArray&) { return ValueType::ARRAY; }
    ValueType operator () (const RwProc&) { return ValueType::PROC; }
    ValueType operator () (const RwSubr&) { return ValueType::SUBR; }
    ValueType operator () (const RwMutPair&) { return ValueType::MUT_PAIR; }
    ValueType operator () (const RwThunk&) { return ValueType::THUNK; }
    ValueType operator () (const String&) { return ValueType::STRING; }
    ValueType operator () (const Symbol&) { return ValueType::SYMBOL; }
    ValueType operator () (const Pair&) { return ValueType::PAIR; }
    ValueType operator () (const Array&) { return ValueType::ARRAY; }
    ValueType operator () (const Proc&) { return ValueType::PROC; }
    ValueType operator () (const Subr&) { return ValueType::SUBR; }
    ValueType operator () (const Ref&) { return ValueType::REF; }
    ValueType operator () (const MutPair&) { return ValueType::MUT_PAIR; }
    ValueType operator () (const Thunk&) { return ValueType::THUNK; }
  };

  return std::visit(TypeVisitor{}, *value);
}

bool is_expval (const Value& value) {
  return type_of(value) != ValueType::THUNK;
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

    void operator () (Unit) { os << "Unit"; }
    void operator () (Nil) { os << "()"; }
    void operator () (Bool b) { os << (b.get() ? "#t" : "#f"); }
    void operator () (Int i) { os << i.get(); }
    void operator () (Double d) { os << d.get(); }
    void operator () (const RwString& str) { (*this)(str.get()); }
    void operator () (const RwSymbol& sym) { (*this)(sym.get()); }
    void operator () (const RwPair& pair) { (*this)(pair.get()); }
    void operator () (const RwArray& array) { (*this)(array.get()); }
    void operator () (const RwProc& proc) { (*this)(proc.get()); }
    void operator () (const RwSubr& subr) { (*this)(subr.get()); }
    void operator () (const RwMutPair& pair) { (*this)(pair.get()); }
    void operator () (const RwThunk& thunk) { (*this)(thunk.get()); }
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
      interleave(std::begin(array.refs),
                 std::end(array.refs),
                 [this, store=array.store] (const Ref& item) {
                   std::visit(OutputVisitor{os}, *store->deref(item));
                 },
                 [this] () { os << ", "; });
      os << ']';
    }
    void operator () (const Proc& proc) {
      os << "<proc " << proc.params() << ">";
    }

    void operator () (const Ref& ref) {
      os << "<ref " << ref.location() << ">";
    }

    void operator () (const MutPair& pair) {
      if (!open_paren) {
        open_paren = true;
        os << '(';
      }

      std::visit(OutputVisitor{os}, *pair.left());

      if (auto type = type_of(pair.right()); type == ValueType::PAIR) {
        os << ' ';
        std::visit(*this, *pair.right());
      } else if (type == ValueType::NIL) {
        os << ')';
      } else {
        os << " . ";
        std::visit(*this, *pair.right());
        os << ')';
      }
    }

    void operator () (const Thunk& thunk) {
      os << "<thunk " << std::hex << (uint64_t)&thunk << ">";
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

const MutPair& to_mut_pair (const Value& value) {
  return std::get<RwMutPair>(*value).get();
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

const Subr& to_subr (const Value& value) {
  return std::get<RwSubr>(*value).get();
}

Subr& to_subr (Value& value) {
  return std::get<RwSubr>(*value).get();
}

const Ref& to_ref (const Value& value) {
  return std::get<Ref>(*value);
}

Ref& to_ref (Value& value) {
  return std::get<Ref>(*value);
}

const Thunk& to_thunk (const Value& value) {
  return std::get<RwThunk>(*value).get ();
}


std::ostream& operator << (std::ostream& os, const Proc& proc) {
  os << "Proc(params_: " << proc.params_
     << ", body: " << proc.body_
     << ", saved_env: " << proc.saved_env() << ")";
  return os;
}

Proc::Proc (const std::vector<Symbol>& params, Expression body, SpEnv saved_env)
    : params_(params), body_(std::move(body)), saved_env_(std::move(saved_env)) { }

Subr::Subr (const std::vector<Symbol>& params, Statement body, SpEnv saved_env)
    : params_(params), body_(std::move(body)), saved_env_(std::move(saved_env)) { }

std::optional<std::vector<Value>> flatten (Value lst) {
  std::vector<Value> values;
  while (true) {
    if (auto type = type_of(lst); type == ValueType::PAIR) {
      auto& pair = to_pair(lst);
      values.push_back(pair.first);
      lst = pair.second;
    } else if (type == ValueType::NIL) {
      break;
    } else {
      return {};
    }
  }
  return {std::move(values)};
}

std::ostream& operator << (std::ostream& os, const Ref& rhs) {
  os << "Ref(" << rhs.location() << ")";
  return os;
}

int Ref::location () const {
  return location_;
}

std::ostream& operator << (std::ostream& os, const Thunk& thunk) {
  os << "Thunk(exp: " << thunk.exp << ", env: " << thunk.env << ")";
  return os;
}

MutPair::MutPair (Ref left_ref, SpStore store)
    : left_ref_(left_ref), store_(std::move(store)) { }

Value MutPair::left () const {
  return store_->deref(left_ref());
}

Value MutPair::right () const {
  return store_->deref(right_ref());
}

Ref MutPair::left_ref () const {
  return left_ref_;
}

Ref MutPair::right_ref () const {
  return Ref{left_ref().location() + 1};
}

void MutPair::left (Value value) {
  store_->setref(left_ref(), std::move(value));
}

void MutPair::right (Value value) {
  store_->setref(right_ref(), std::move(value));
}


}