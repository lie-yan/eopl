//
// Created by robin on 2019-01-12.
//

#include "built_in.h"

#include "value.h"
#include <numeric>
#include <gsl/gsl>

namespace eopl::built_in {

std::optional<BuiltInFun> find_built_in (const Symbol& name) {
  static std::map<Symbol, BuiltInFun> fun_table = {
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
  Expects(args.size() == 1);
  auto i = to_int(args[0]);
  auto res = Int{-i.get()};
  return to_value(res);
}

Value diff (const std::vector<Value>& args) {
  Expects(args.size() == 2);
  auto i1 = to_int(args[0]);
  auto i2 = to_int(args[1]);
  auto res = Int{i1.get() - i2.get()};
  return to_value(res);
}

Value sum (const std::vector<Value>& args) {
  Expects(args.size() >= 2);
  int res = std::accumulate(std::begin(args),
                            std::end(args),
                            0,
                            [] (int acc, const Value& val) {
                              return acc + to_int(val).get();
                            });
  return to_value(Int{res});
}

Value mult (const std::vector<Value>& args) {
  Expects(args.size() >= 2);
  int res = std::accumulate(std::begin(args),
                            std::end(args),
                            1,
                            [] (int acc, const Value& val) {
                              return acc * to_int(val).get();
                            });
  return to_value(Int{res});
}

Value divide (const std::vector<Value>& args) {
  Expects(args.size() == 2);
  auto i1 = to_int(args[0]);
  auto i2 = to_int(args[1]);
  auto res = Int{i1.get() / i2.get()};
  return to_value(res);
}

Value zero_test (const std::vector<Value>& args) {
  Expects(args.size() == 1);
  auto i = to_int(args[0]);
  auto res = Bool{i.get() == 0};
  return to_value(res);
}

Value equal_test (const std::vector<Value>& args) {
  Expects(args.size() == 2);
  auto i1 = to_int(args[0]);
  auto i2 = to_int(args[1]);
  auto res = Bool{i1.get() == i2.get()};
  return to_value(res);
}

Value greater_test (const std::vector<Value>& args) {
  Expects(args.size() == 2);
  auto i1 = to_int(args[0]);
  auto i2 = to_int(args[1]);
  auto res = Bool{i1.get() > i2.get()};
  return to_value(res);
}

Value less_test (const std::vector<Value>& args) {
  Expects(args.size() == 2);
  auto i1 = to_int(args[0]);
  auto i2 = to_int(args[1]);
  auto res = Bool{i1.get() < i2.get()};
  return to_value(res);
}

Value cons (const std::vector<Value>& args) {
  Expects(args.size() == 2);
  auto res = Pair{args[0], args[1]};
  return to_value(std::move(res));
}

Value car (const std::vector<Value>& args) {
  Expects(args.size() == 1);
  if (type_of(args[0]) == ValueType::PAIR) {
    return to_pair(args[0]).first;
  } else {
    throw std::runtime_error("Pair expected");
  }
}

Value cdr (const std::vector<Value>& args) {
  Expects(args.size() == 1);
  if (type_of(args[0]) == ValueType::PAIR) {
    return to_pair(args[0]).second;
  } else {
    throw std::runtime_error("Pair expected");
  }
}

Value null_test (const std::vector<Value>& args) {
  Expects(args.size() == 1);
  auto res = Bool{type_of(args[0]) == ValueType::NIL};
  return to_value(res);
}

Value list (const std::vector<Value>& args) {
  Value res = std::accumulate(std::rbegin(args),
                              std::rend(args),
                              to_value(Nil()),
                              [] (Value& acc, Value value) -> Value {
                                auto t = Pair{std::move(value), std::move(acc)};
                                return to_value(std::move(t));
                              });
  return res;
}

}