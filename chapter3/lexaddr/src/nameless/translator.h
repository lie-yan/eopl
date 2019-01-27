//
// Created by Lie Yan on 2019-01-27.
//

#pragma once

#include "expr.h"

namespace eopl {

Program translation_of (const Program& program);

Expression translation_of (const Expression& exp, SpStaticEnv env);
Expression translation_of (const ConstExp& exp, SpStaticEnv env);
Expression translation_of (const VarExp& exp, SpStaticEnv env);
Expression translation_of (const NamelessVarExp& exp, SpStaticEnv env);
Expression translation_of (const IfExp& exp, SpStaticEnv env);
Expression translation_of (const LetExp& exp, SpStaticEnv env);
Expression translation_of (const NamelessLetExp& exp, SpStaticEnv env);
Expression translation_of (const CondExp& exp, SpStaticEnv env);
Expression translation_of (const UnpackExp& exp, SpStaticEnv env);
Expression translation_of (const ProcExp& exp, SpStaticEnv env);
Expression translation_of (const NamelessProcExp& exp, SpStaticEnv env);
Expression translation_of (const CallExp& exp, SpStaticEnv env);
Expression translation_of (const LetrecExp& exp, SpStaticEnv env);

std::vector<Expression> translation_of (const std::vector<Expression>& exps, SpStaticEnv env);


}