//
// Created by robin on 2019-01-01.
//

#pragma once

#include <memory>
#include <utility>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <numeric>

namespace eopl {

struct SymbolNotFoundError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

template<typename Symbol, typename Value, typename Expression>
//   requires Regular<Symbol>, Semiregular<Value>
class Environment {
public:

  struct ForOrd {
    Symbol symbol;
    Value value;
  };

  struct ForRec {
    Symbol proc_name;
    std::vector<Symbol> bound_vars;
    Expression body;
  };

  using SVPair = std::pair<Symbol, Value>;
  using SpEnv = std::shared_ptr<Environment>;

  Environment () = default;
  Environment (const Environment&) = delete;
  Environment& operator = (const Environment&) = delete;
  Environment (Environment&&) = delete;
  Environment& operator = (Environment&&) = delete;

  Environment (SpEnv parent, ForOrd pair)
      : parent_(std::move(parent)), bound_record_(std::move(pair)) { }

  Environment (SpEnv parent, ForRec triple)
      : parent_(std::move(parent)), bound_record_(std::move(triple)) { }

  static SpEnv make_empty () { return SpEnv(); }

  static SpEnv extend (Environment::SpEnv parent, Symbol sym, Value value) {
    return std::make_shared<Environment>(std::move(parent),
                                         ForOrd{std::move(sym), std::move(value)});
  }

  static SpEnv extend (Environment::SpEnv parent,
                       std::vector<Symbol> syms,
                       std::vector<Value> values) {
    assert(syms.size() == values.size());
    return std::inner_product(std::begin(syms),
                              std::end(syms),
                              std::begin(values),
                              parent,
                              [] (auto&& acc, auto&& pair) {
                                return extend(std::forward<decltype(acc)>(acc),
                                              std::move(pair.symbol),
                                              std::move(pair.value));
                              },
                              [] (auto&& sym, auto&& val) {
                                return ForOrd{std::forward<decltype(sym)>(sym),
                                              std::forward<decltype(val)>(val)};
                              });
  }

  static SpEnv extend_rec (Environment::SpEnv parent, Symbol proc_name,
                           std::vector<Symbol> bound_vars, Expression body) {
    return std::make_shared<Environment>(std::move(parent),
                                         ForRec{std::move(proc_name),
                                                std::move(bound_vars),
                                                std::move(body)});
  }

  static Value apply (Environment::SpEnv env, const Symbol& sym) {

    for (auto p = env; p; p = p->parent_) {
      auto& bound_record = p->bound_record_;
      if (std::holds_alternative<ForOrd>(bound_record)) {
        if (auto& ord = std::get<ForOrd>(bound_record); ord.symbol == sym) {
          return ord.value;
        }
      } else if (std::holds_alternative<ForRec>(bound_record)) {
        if (auto& rec = std::get<ForRec>(bound_record); rec.proc_name == sym) {
          return to_value(Proc{rec.bound_vars, rec.body, env});
        }
      }
    }
    throw SymbolNotFoundError(fmt::format("Symbol {} not found.", sym));
  }

private:
  SpEnv parent_;

  std::variant<ForOrd, ForRec> bound_record_;
};

}
