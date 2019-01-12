//
// Created by robin on 2019-01-12.
//

#include "builtin.h"


namespace eopl::builtin {

std::optional<BuiltinFun> find_builtin (const Symbol& name) {
  static std::map<Symbol, BuiltinFun> fun_table = {
      {Symbol{"minus"}, minus},
      {Symbol{"zero?"}, zero_test},
      {Symbol{"equal?"}, equal_test},
      {Symbol{"greater?"}, greater_test},
      {Symbol{"less?"}, less_test},
      {Symbol{"-"}, diff}
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

Value diff (const std::vector<Value>& args) {
  assert(args.size() == 2);
  auto i1 = value_to_int(args[0]);
  auto i2 = value_to_int(args[1]);
  return Int{i1.get() - i2.get()};
}


}