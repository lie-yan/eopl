//
// Created by robin on 2019-01-12.
//

#pragma once

#include <optional>
#include "value_fwd.h"
#include "store.h"

namespace eopl::built_in {

using BuiltInFun = std::function<Value (const std::vector<Value>&, const SpStore&)>;

/**
 * @brief Given a fixed symbol, return the function corresponding to
 *        the symbol if any; otherwise return None.
 *
 * @param name the name to search for
 * @return the function if any, or None
 */
std::optional<BuiltInFun> find_built_in (const Symbol& name);

Value minus (const std::vector<Value>& args, const SpStore& store);
Value diff (const std::vector<Value>& args, const SpStore& store);
Value sum (const std::vector<Value>& args, const SpStore& store);
Value mult (const std::vector<Value>& args, const SpStore& store);
Value divide (const std::vector<Value>& args, const SpStore& store);
Value zero_test (const std::vector<Value>& args, const SpStore& store);
Value equal_test (const std::vector<Value>& args, const SpStore& store);
Value greater_test (const std::vector<Value>& args, const SpStore& store);
Value less_test (const std::vector<Value>& args, const SpStore& store);

Value cons (const std::vector<Value>& args, const SpStore& store);
Value car (const std::vector<Value>& args, const SpStore& store);
Value cdr (const std::vector<Value>& args, const SpStore& store);
Value null_test (const std::vector<Value>& args, const SpStore& store);
Value list (const std::vector<Value>& args, const SpStore& store);

Value newref (const std::vector<Value>& args, const SpStore& store);
Value deref (const std::vector<Value>& args, const SpStore& store);
Value setref (const std::vector<Value>& args, const SpStore& store);

}