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
std::optional<BuiltInFun> find_function (const Symbol& name);

Value minus (const std::vector<Value>& args, const SpStore& store);
Value diff (const std::vector<Value>& args, const SpStore& store);
Value sum (const std::vector<Value>& args, const SpStore& store);
Value mult (const std::vector<Value>& args, const SpStore& store);
Value not_ (const std::vector<Value>& args, const SpStore& store);
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

Value pair (const std::vector<Value>& args, const SpStore& store);
Value left (const std::vector<Value>& args, const SpStore& store);
Value right (const std::vector<Value>& args, const SpStore& store);
Value setleft (const std::vector<Value>& args, const SpStore& store);
Value setright (const std::vector<Value>& args, const SpStore& store);

Value newarray (const std::vector<Value>& args, const SpStore& store);
Value arrayref (const std::vector<Value>& args, const SpStore& store);
Value arrayset (const std::vector<Value>& args, const SpStore& store);
Value arraylength (const std::vector<Value>& args, const SpStore& store);

std::optional<BuiltInFun> find_subroutine (const Symbol& name);
Value print (const std::vector<Value>& args, const SpStore& store);

}