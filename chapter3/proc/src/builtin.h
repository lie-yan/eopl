//
// Created by robin on 2019-01-12.
//

#pragma once

#include <optional>
#include "value.h"

namespace eopl::builtin {

using BuiltinFun = std::function<Value (const std::vector<Value>&)>;

std::optional<BuiltinFun> find_builtin (const Symbol& name);

Value minus (const std::vector<Value>& args);
Value diff (const std::vector<Value>& args);
Value sum (const std::vector<Value>& args);
Value mult (const std::vector<Value>& args);
Value divide (const std::vector<Value>& args);
Value zero_test (const std::vector<Value>& args);
Value equal_test (const std::vector<Value>& args);
Value greater_test (const std::vector<Value>& args);
Value less_test (const std::vector<Value>& args);

Value cons (const std::vector<Value>& args);
Value car (const std::vector<Value>& args);
Value cdr (const std::vector<Value>& args);
Value null_test (const std::vector<Value>& args);
Value list (const std::vector<Value>& args);

}