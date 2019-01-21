//
// Created by robin on 2019-01-10.
//

#pragma once

#include "value.h"
#include "expr.h"

namespace eopl {

//using Env = Environment<Symbol, Value>;
//using SpEnv = Env::SpEnv;

Value value_of (const Program& program, SpEnv env);
Value value_of (const Expression& exp, SpEnv env);
Value value_of (const ConstExp& exp, SpEnv env);
Value value_of (const VarExp& exp, SpEnv env);
Value value_of (const IfExp& exp, SpEnv env);
Value value_of (const LetExp& exp, SpEnv env);
Value value_of (const OpExp& exp, SpEnv env);
Value value_of (const CondExp& exp, SpEnv env);
Value value_of (const UnpackExp& exp, SpEnv env);
Value value_of (const ProcExp& exp, SpEnv env);
Value value_of (const CallExp& exp, SpEnv env);

std::vector<Value> value_of (const std::vector<Expression>& exps, SpEnv env);

SpEnv make_initial_env ();
Value eval (const std::string& s);

}