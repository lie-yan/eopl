//
// Created by robin on 2019-01-12.
//

#include "builtin.h"

#include <numeric>

namespace eopl::builtin {

std::optional<BuiltinFun> find_builtin (const Symbol& name) {
  static std::map<Symbol, BuiltinFun> fun_table = {
      {Symbol{"zero?"},    zero_test},
      {Symbol{"equal?"},   equal_test},
      {Symbol{"greater?"}, greater_test},
      {Symbol{"less?"},    less_test},
      {Symbol{"minus"},    minus},
      {Symbol{"-"},        diff},
      {Symbol{"+"},        sum},
      {Symbol{"*"},        mult},
      {Symbol{"/"},        divide},
      {Symbol{"cons"},     cons},
      {Symbol{"car"},      car},
      {Symbol{"cdr"},      cdr},
      {Symbol{"null?"},    null_test},
      {Symbol{"list"},     list},
  };

  if (auto it = fun_table.find(name); it != std::end(fun_table)) {
    return {it->second};
  } else {
    return {};
  }
}

Value minus (const std::vector<Value>& args) {
  assert(args.size() == 1);
  auto i = value_to_int(args[0]);
  return Int{-i.get()};
}

Value diff (const std::vector<Value>& args) {
  assert(args.size() == 2);
  auto i1 = value_to_int(args[0]);
  auto i2 = value_to_int(args[1]);
  return Int{i1.get() - i2.get()};
}

Value sum (const std::vector<Value>& args) {
  assert(args.size() >= 2);
  int res = std::accumulate(std::begin(args),
                            std::end(args),
                            0,
                            [] (int acc, const Value& val) {
                              return acc + value_to_int(val).get();
                            });
  return Int{res};
}

Value mult (const std::vector<Value>& args) {
  assert(args.size() >= 2);
  int res = std::accumulate(std::begin(args),
                            std::end(args),
                            1,
                            [] (int acc, const Value& val) {
                              return acc * value_to_int(val).get();
                            });
  return Int{res};
}

Value divide (const std::vector<Value>& args) {
  assert(args.size() == 2);
  auto i1 = value_to_int(args[0]);
  auto i2 = value_to_int(args[1]);
  return Int{i1.get() / i2.get()};
}

Value zero_test (const std::vector<Value>& args) {
  assert(args.size() == 1);
  auto i = value_to_int(args[0]);
  return Bool{i.get() == 0};
}

Value equal_test (const std::vector<Value>& args) {
  assert(args.size() == 2);
  auto i1 = value_to_int(args[0]);
  auto i2 = value_to_int(args[1]);
  return Bool{i1.get() == i2.get()};
}

Value greater_test (const std::vector<Value>& args) {
  assert(args.size() == 2);
  auto i1 = value_to_int(args[0]);
  auto i2 = value_to_int(args[1]);
  return Bool{i1.get() > i2.get()};
}

Value less_test (const std::vector<Value>& args) {
  assert(args.size() == 2);
  auto i1 = value_to_int(args[0]);
  auto i2 = value_to_int(args[1]);
  return Bool{i1.get() < i2.get()};
}

Value cons (const std::vector<Value>& args) {
  assert(args.size() == 2);
  return Pair{args[0], args[1]};
}
Value car (const std::vector<Value>& args) {
  assert(args.size() == 1);
  if (type_of(args[0]) == ValueType::PAIR) {
    return std::get<RwPair>(args[0]).get().first;
  } else {
    throw std::runtime_error("Pair expected");
  }
}
Value cdr (const std::vector<Value>& args) {
  assert(args.size() == 1);
  if (type_of(args[0]) == ValueType::PAIR) {
    return std::get<RwPair>(args[0]).get().second;
  } else {
    throw std::runtime_error("Pair expected");
  }
}

Value null_test (const std::vector<Value>& args) {
  assert(args.size() == 1);
  return Bool{type_of(args[0]) == ValueType::NIL};
}

Value list (const std::vector<Value>& args) {
  Value res = std::accumulate(std::rbegin(args),
                              std::rend(args),
                              Value(Nil{}),
                              [] (Value acc, Value value) -> Value {
                                return Pair{std::move(value), std::move(acc)};
                              });
  return res;
}

}