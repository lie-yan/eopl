//
// Created by Lie Yan on 2019-01-27.
//

#pragma once

#include "expr.h"

namespace eopl {

Program translation_of (const Program& program, const SpStaticEnv& senv);

Expression translation_of (const Expression& exp, const SpStaticEnv& senv);
Expression translation_of (const ConstExp& exp, const SpStaticEnv& senv);
Expression translation_of (const VarExp& exp, const SpStaticEnv& senv);
Expression translation_of (const NamelessVarExp& exp, const SpStaticEnv& senv);
Expression translation_of (const IfExp& exp, const SpStaticEnv& senv);
Expression translation_of (const LetExp& exp, const SpStaticEnv& senv);
Expression translation_of (const NamelessLetExp& exp, const SpStaticEnv& senv);
Expression translation_of (const CondExp& exp, const SpStaticEnv& senv);
Expression translation_of (const UnpackExp& exp, const SpStaticEnv& senv);
Expression translation_of (const NamelessUnpackExp& exp, const SpStaticEnv& senv);
Expression translation_of (const ProcExp& exp, const SpStaticEnv& senv);
Expression translation_of (const NamelessProcExp& exp, const SpStaticEnv& senv);
Expression translation_of (const CallExp& exp, const SpStaticEnv& senv);
Expression translation_of (const LetrecExp& exp, const SpStaticEnv& senv);
Expression translation_of (const NamelessLetrecExp& exp, const SpStaticEnv& senv);

std::vector<Expression> translation_of (const std::vector<Expression>& exps, const SpStaticEnv& senv);

SpStaticEnv make_initial_senv ();

}