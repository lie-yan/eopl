//
// Created by robin on 2019-01-12.
//

#pragma once

#include <optional>
#include "value.h"

namespace eopl::built_in {

using BuiltInFun = std::function<Value (const std::vector<Value>&)>;

std::optional<BuiltInFun> find_built_in (const Symbol& name);

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