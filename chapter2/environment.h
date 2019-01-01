//
// Created by robin on 2019-01-01.
//

#pragma once

#include <memory>
#include <utility>
#include "value.h"

namespace eopl {

class Environment : public std::enable_shared_from_this<Environment> {
public:
  using SpEnv = std::shared_ptr<Environment>;

  struct SymbolNotFoundError : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  Environment () = default;
  Environment (const Environment&) = delete;
  Environment& operator = (const Environment&) = delete;
  Environment (Environment&&) = delete;
  Environment& operator = (Environment&&) = delete;

  Environment (SpEnv parent, std::pair<Symbol, Value> pair);

  static SpEnv make_empty ();
  static SpEnv extend (SpEnv parent, Symbol sym, Value value);
  static const Value& apply(SpEnv env, const Symbol& sym);

private:
  SpEnv parent_;
  std::pair<Symbol, Value> pair_;
};

}
