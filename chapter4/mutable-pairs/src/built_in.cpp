//
// Created by robin on 2019-01-12.
//

#include "built_in.h"

#include "value.h"
#include "basic.h"
#include <numeric>
#include <gsl/gsl>
#include <iostream>

namespace eopl::built_in {

std::optional<BuiltInFun> find_function (const Symbol& name) {
  static std::map<Symbol, BuiltInFun> fun_table = {
      {Symbol{"zero?"},    zero_test},
      {Symbol{"equal?"},   equal_test},
      {Symbol{"greater?"}, greater_test},
      {Symbol{"less?"},    less_test},
      {Symbol{"minus"},    minus},
      {Symbol{"not"},      not_},
      {Symbol{"-"},        diff},
      {Symbol{"+"},        sum},
      {Symbol{"*"},        mult},
      {Symbol{"/"},        divide},
      {Symbol{"cons"},     cons},
      {Symbol{"car"},      car},
      {Symbol{"cdr"},      cdr},
      {Symbol{"null?"},    null_test},
      {Symbol{"list"},     list},
      {Symbol{"newref"},   newref},
      {Symbol{"deref"},    deref},
      {Symbol{"setref"},   setref},
      {Symbol{"pair"},     pair},
      {Symbol{"left"},     left},
      {Symbol{"right"},    right},
      {Symbol{"setleft"},  setleft},
      {Symbol{"setright"}, setright},
  };

  if (auto it = fun_table.find(name); it != std::end(fun_table)) {
    return {it->second};
  } else {
    return {};
  }
}

Value minus (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 1);
  auto i = to_int(args[0]);
  auto res = Int{-i.get()};
  return to_value(res);
}

Value not_ (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 1);
  auto b = to_bool(args[0]);
  auto res = Bool{!b.get()};
  return to_value(res);
}

Value diff (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 2);
  auto i1 = to_int(args[0]);
  auto i2 = to_int(args[1]);
  auto res = Int{i1.get() - i2.get()};
  return to_value(res);
}

Value sum (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() >= 2);
  int res = std::accumulate(std::begin(args),
                            std::end(args),
                            0,
                            [] (int acc, const Value& val) {
                              return acc + to_int(val).get();
                            });
  return to_value(Int{res});
}

Value mult (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() >= 2);
  int res = std::accumulate(std::begin(args),
                            std::end(args),
                            1,
                            [] (int acc, const Value& val) {
                              return acc * to_int(val).get();
                            });
  return to_value(Int{res});
}

Value divide (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 2);
  auto i1 = to_int(args[0]);
  auto i2 = to_int(args[1]);
  auto res = Int{i1.get() / i2.get()};
  return to_value(res);
}

Value zero_test (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 1);
  auto i = to_int(args[0]);
  auto res = Bool{i.get() == 0};
  return to_value(res);
}

Value equal_test (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 2);
  auto i1 = to_int(args[0]);
  auto i2 = to_int(args[1]);
  auto res = Bool{i1.get() == i2.get()};
  return to_value(res);
}

Value greater_test (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 2);
  auto i1 = to_int(args[0]);
  auto i2 = to_int(args[1]);
  auto res = Bool{i1.get() > i2.get()};
  return to_value(res);
}

Value less_test (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 2);
  auto i1 = to_int(args[0]);
  auto i2 = to_int(args[1]);
  auto res = Bool{i1.get() < i2.get()};
  return to_value(res);
}

Value cons (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 2);
  auto res = Pair{args[0], args[1]};
  return to_value(std::move(res));
}

Value car (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 1);
  if (type_of(args[0]) == ValueType::PAIR) {
    return to_pair(args[0]).first;
  } else {
    throw std::runtime_error("Pair expected");
  }
}

Value cdr (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 1);
  if (type_of(args[0]) == ValueType::PAIR) {
    return to_pair(args[0]).second;
  } else {
    throw std::runtime_error("Pair expected");
  }
}

Value null_test (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 1);
  auto res = Bool{type_of(args[0]) == ValueType::NIL};
  return to_value(res);
}

Value list (const std::vector<Value>& args, const SpStore& store) {
  Value res = std::accumulate(std::rbegin(args),
                              std::rend(args),
                              to_value(Nil()),
                              [] (Value& acc, Value value) -> Value {
                                auto t = Pair{std::move(value), std::move(acc)};
                                return to_value(std::move(t));
                              });
  return res;
}

Value newref (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 1);
  return to_value(store->newref(args[0]));
}

Value deref (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 1);
  return store->deref(to_ref(args[0]));
}

Value setref (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 2);
  return store->setref(to_ref(args[0]), args[1]);
}

void print (const std::vector<Value>& args, const SpStore& store) {
  interleave(
      std::begin(args),
      std::end(args),
      [] (const Value& val) { std::cout << val; },
      [] () { std::cout << ","; }
  );
  std::cout << std::endl;
}

std::optional<BuiltInSubr> find_subroutine (const Symbol& name) {
  static std::map<Symbol, BuiltInSubr> subr_table = {
      {Symbol{"print"}, print}
  };

  if (auto it = subr_table.find(name); it != std::end(subr_table)) {
    return {it->second};
  } else {
    return {};
  }
}

Value pair (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 2);

  auto left_ref = store->newref(args[0]);
  store->newref(args[1]);
  return
      to_value(MutPair{left_ref,
                       store});
}
Value left (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 1);
  return
    to_mut_pair(args[0]).left();
}

Value right (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 1);
  return
    to_mut_pair(args[0]).right();
}

Value setleft (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 2);
  store->setref(to_mut_pair(args[0]).left_ref(), args[1]);
  return to_value(Int{38});
}

Value setright (const std::vector<Value>& args, const SpStore& store) {
  Expects(args.size() == 2);
  store->setref(to_mut_pair(args[0]).right_ref(), args[1]);
  return to_value(Int{38});
}


}