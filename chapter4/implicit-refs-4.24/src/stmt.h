//
// Created by Lie Yan on 2019-02-03.
//

#pragma once

#include <ostream>
#include "stmt_fwd.h"

namespace eopl {

struct BlockStmt {
  std::vector<Statement> stmt_list;

  friend std::ostream& operator << (std::ostream& os, const BlockStmt& stmt);
};

struct IfStmt {
  Expression cond;
  Statement then_;
  Statement else_;

  friend std::ostream& operator << (std::ostream& os, const IfStmt& stmt);
};

struct WhileStmt {
  Expression cond;
  Statement body;

  friend std::ostream& operator << (std::ostream& os, const WhileStmt& stmt);
};

struct DeclStmt {
  std::vector<Symbol> vars;
  Statement body;

  friend std::ostream& operator << (std::ostream& os, const DeclStmt& stmt);
};

struct DoWhileStmt {
  Expression cond;
  Statement body;

  friend std::ostream& operator << (std::ostream& os, const DoWhileStmt& stmt);
};

struct Program {
  Statement stmt;

  friend std::ostream& operator << (std::ostream& os, const Program& program);
};


}