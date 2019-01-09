//
// Created by robin on 2019-01-01.
//

#pragma once

#include <memory>
#include <utility>
#include <fmt/format.h>

namespace eopl {

template<typename Symbol, typename Value>
//   requires Regular<Symbol>, Semiregular<Value>
class Env {
public:
  using SpEnv = std::shared_ptr<Env>;

  struct SymbolNotFoundError : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  Env () = default;
  Env (const Env&) = delete;
  Env& operator = (const Env&) = delete;
  Env (Env&&) = delete;
  Env& operator = (Env&&) = delete;

  Env (SpEnv parent, std::pair<Symbol, Value> pair)
      : parent_(std::move(parent)), pair_(std::move(pair)) { }

  static SpEnv make_empty () { return SpEnv(); }

  static SpEnv extend (Env::SpEnv parent, Symbol sym, Value value) {
    return std::make_shared<Env>(std::move(parent),
                                 std::pair(std::move(sym), std::move(value)));
  }

  static const Value& apply (Env::SpEnv env, const Symbol& sym) {
    for (; env; env = env->parent_) {
      if (env->pair_.first == sym) return env->pair_.second;
    }
    throw SymbolNotFoundError(fmt::format("Symbol {} not found.", sym));
  }

private:
  SpEnv parent_;
  std::pair<Symbol, Value> pair_;
};

}
