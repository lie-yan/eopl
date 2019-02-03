//
// Created by Lie Yan on 2019-02-03.
//

#include "stmt.h"

#include "basic.h"
#include "stmt_fwd.h"

namespace eopl {

struct StatementOutputVisitor {
  std::ostream& os;

  template<typename T>
  void operator () (const boost::recursive_wrapper<T>& t) { os << t.get(); }

  template<typename T>
  void operator () (const T& t) { os << t; }
};

std::ostream& operator << (std::ostream& os, const Statement& stmt) {
  std::visit(StatementOutputVisitor{os}, *stmt);
  return os;
}

std::ostream& operator << (std::ostream& os, const AssignStmt& stmt) {
  os << "AssignStmt(var: " << stmt.var
     << ", exp: " << stmt.exp << ")";
  return os;
}

template<typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& ts) {
  interleave(std::begin(ts),
             std::end(ts),
             [&os] (const auto& x) { os << x; },
             [&os] () { os << ", "; });
  return os;
}

std::ostream& operator << (std::ostream& os, const SubrCallStmt& stmt) {
  os << "SubrCallStmt(var: " << stmt.var
     << ", params: " << stmt.params << ")";
  return os;
}

StmtType type_of (const Statement& statement) {
  struct TypeVisitor {

    StmtType operator () (const AssignStmt&) { return StmtType::ASSIGN_STMT; }
    StmtType operator () (const SubrCallStmt&) { return StmtType::SUBR_CALL_STMT; }
    StmtType operator () (const RwBlockStmt&) { return StmtType::BLOCK_STMT; }
    StmtType operator () (const RwIfStmt&) { return StmtType::IF_STMT; }
    StmtType operator () (const RwWhileStmt&) { return StmtType::WHILE_STMT; }
    StmtType operator () (const RwDeclStmt&) { return StmtType::DECL_STMT; }
    StmtType operator () (const ReadStmt&) { return StmtType::READ_STMT; }
  };

  return std::visit(TypeVisitor{}, *statement);
}

std::ostream& operator << (std::ostream& os, const BlockStmt& stmt) {
  os << "BlockStmt(stmt_list: " << stmt.stmt_list << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const IfStmt& stmt) {
  os << "IfStmt(cond: " << stmt.cond
     << ", then_: " << stmt.then_
     << ", else_: " << stmt.else_ << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const WhileStmt& stmt) {
  os << "WhileStmt(cond: " << stmt.cond
     << ", body: " << stmt.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const DeclStmt& stmt) {
  os << "DeclStmt(vars: " << stmt.vars
     << ", body: " << stmt.body << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const ReadStmt& stmt) {
  os << "ReadStmt(var: " << stmt.var << ")";
  return os;
}

std::ostream& operator << (std::ostream& os, const Program& program) {
  os << "Program(stmt: " << program.stmt << ")";
  return os;
}


}