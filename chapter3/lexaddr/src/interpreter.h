//
// Created by robin on 2019-01-10.
//

#pragma once

#include "value.h"
#include "expr.h"
#include "env.h"

namespace eopl {

Value value_of (const Program& program, const SpEnv& env);
Value value_of (const Expression& exp, const SpEnv& env);
Value value_of (const ConstExp& exp, const SpEnv& env);
Value value_of (const VarExp& exp, const SpEnv& env);
Value value_of (const NamelessVarExp& exp, const SpEnv& env);
Value value_of (const IfExp& exp, const SpEnv& env);
Value value_of (const LetExp& exp, const SpEnv& env);
Value value_of (const NamelessLetExp& exp, const SpEnv& env);
Value value_of (const CondExp& exp, const SpEnv& env);
Value value_of (const UnpackExp& exp, const SpEnv& env);
Value value_of (const NamelessUnpackExp& exp, const SpEnv& env);
Value value_of (const ProcExp& exp, const SpEnv& env);
Value value_of (const NamelessProcExp& exp, const SpEnv& env);
Value value_of (const CallExp& exp, const SpEnv& env);
Value value_of (const LetrecExp& exp, const SpEnv& env);
Value value_of (const NamelessLetrecExp& exp, const SpEnv& env);

std::vector<Value> value_of (const std::vector<Expression>& exps, const SpEnv& env);

SpEnv make_initial_env ();
Value eval (const std::string& s);

}