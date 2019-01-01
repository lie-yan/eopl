#include <utility>

//
// Created by robin on 2019-01-01.
//

#pragma once

#include <memory>
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
  const Value& apply (const Symbol& sym);
  SpEnv extend (Symbol sym, Value value);

private:
  SpEnv parent_;
  std::pair<Symbol, Value> pair_;
};

}
