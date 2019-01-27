//
// Created by Lie Yan on 2019-01-27.
//

#pragma once

#include "value.h"
#include "expr.h"
#include "nenv.h"

namespace eopl {

Value nameless_value_of (const Program& program, const SpNamelessEnv& nenv);
Value nameless_value_of (const Expression& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const ConstExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const VarExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const NamelessVarExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const IfExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const LetExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const NamelessLetExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const CondExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const UnpackExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const ProcExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const NamelessProcExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const CallExp& exp, const SpNamelessEnv& nenv);
Value nameless_value_of (const LetrecExp& exp, const SpNamelessEnv& nenv);

std::vector<Value> nameless_value_of (const std::vector<Expression>& exps, const SpNamelessEnv& nenv);

SpNamelessEnv make_initial_nenv ();

Value nameless_eval (const std::string& s);


}