//
// Created by Lie Yan on 2019-02-10.
//

#pragma once

#include "tenv.h"
#include "expr_fwd.h"

namespace eopl::type {

Type type_of (const Program& program, const SpTEnv& tenv);
Type type_of (const Expression& expression, const SpTEnv& tenv);
Type type_of (const ConstExp& exp, const SpTEnv& tenv);
Type type_of (const VarExp& exp, const SpTEnv& tenv);
Type type_of (const IfExp& exp, const SpTEnv& tenv);
Type type_of (const LetExp& exp, const SpTEnv& tenv);
Type type_of (const CondExp& exp, const SpTEnv& tenv);
Type type_of (const UnpackExp& exp, const SpTEnv& tenv);
Type type_of (const ProcExp& exp, const SpTEnv& tenv);
Type type_of (const CallExp& exp, const SpTEnv& tenv);
Type type_of (const LetrecExp& exp, const SpTEnv& tenv);

void check_equal_type (const Type& actual, const Type& expected, const Expression& expression);

SpTEnv make_initial_tenv ();
void check_program (const std::string& text);

}