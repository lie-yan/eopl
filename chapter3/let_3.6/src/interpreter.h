//
// Created by robin on 2019-01-10.
//

#pragma once

#include "value.h"
#include "expr.h"
#include "env.h"

namespace eopl {

using Env = Environment<Symbol, Value>;
using SpEnv = Env::SpEnv;

Value value_of (const Program& program, SpEnv env);

Value value_of (const Expression& expression, SpEnv env);

Value value_of (const ConstExp& exp, SpEnv env);

Value value_of (const VarExp& exp, SpEnv env);

Value value_of (const DiffExp& exp, SpEnv env);

Value value_of (const ZeroTestExp& exp, SpEnv env);

Value value_of (const IfExp& exp, SpEnv env);

Value value_of (const LetExp& exp, SpEnv env);

Value value_of (const MinusExp& exp, SpEnv env);

}