//
// Created by robin on 2019-01-10.
//

#pragma once

#include "value.h"
#include "expr.h"
#include "env.h"
#include "store.h"
#include "stmt_fwd.h"

namespace eopl {

Value value_of (const Expression& exp, const SpEnv& env, const SpStore& store);
Value value_of (const ConstExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const VarExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const IfExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const LetExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const CondExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const UnpackExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const ProcExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const CallExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const LetrecExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const SequenceExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const AssignExp& exp, const SpEnv& env, const SpStore& store);
Value value_of (const SetdynamicExp& exp, const SpEnv& env, const SpStore& store);

std::vector<Value> value_of (const std::vector<Expression>& exps, const SpEnv& env, const SpStore& store);

void result_of (const Program& program, const SpEnv& env, const SpStore& store);
void result_of (const Statement& statement, const SpEnv& env, const SpStore& store);
void result_of (const AssignStmt& statement, const SpEnv& env, const SpStore& store);
void result_of (const SubrCallStmt& statement, const SpEnv& env, const SpStore& store);
void result_of (const BlockStmt& statement, const SpEnv& env, const SpStore& store);
void result_of (const IfStmt& statement, const SpEnv& env, const SpStore& store);
void result_of (const WhileStmt& statement, const SpEnv& env, const SpStore& store);
void result_of (const DeclStmt& statement, const SpEnv& env, const SpStore& store);

SpEnv make_initial_env (const SpStore& store);
void run (const std::string& s);

}