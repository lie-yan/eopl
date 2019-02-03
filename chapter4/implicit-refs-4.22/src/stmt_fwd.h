//
// Created by Lie Yan on 2019-02-03.
//

#pragma once

#include <ostream>
#include "expr_fwd.h"

namespace eopl {

enum class StmtType {
  ASSIGN_STMT,
  SUBR_CALL_STMT,
  BLOCK_STMT,
  IF_STMT,
  WHILE_STMT,
  DECL_STMT,
};

struct AssignStmt {
  Symbol var;
  Expression exp;

  friend std::ostream& operator << (std::ostream& os, const AssignStmt& stmt);
};

struct SubrCallStmt {
  Symbol var;
  std::vector<Expression> params;

  friend std::ostream& operator << (std::ostream& os, const SubrCallStmt& stmt);
};

using RwBlockStmt = boost::recursive_wrapper<struct BlockStmt>;
using RwIfStmt = boost::recursive_wrapper<struct IfStmt>;
using RwWhileStmt = boost::recursive_wrapper<struct WhileStmt>;
using RwDeclStmt = boost::recursive_wrapper<struct DeclStmt>;

using Statement_ = std::variant<AssignStmt,
                                SubrCallStmt,
                                RwBlockStmt,
                                RwIfStmt,
                                RwWhileStmt,
                                RwDeclStmt>;

using Statement = std::shared_ptr<Statement_>;

std::ostream& operator << (std::ostream& os, const Statement& stmt);

template<typename T>
Statement to_stmt (T&& t) {
  return std::make_shared<Statement_>(std::forward<T>(t));
}

StmtType type_of (const Statement& statement);

}