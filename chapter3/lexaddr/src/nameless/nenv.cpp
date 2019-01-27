//
// Created by Lie Yan on 2019-01-27.
//

#include <exception.h>
#include "nenv.h"

namespace eopl {

NamelessEnv::NamelessEnv (SpNamelessEnv parent, Value value)
    : parent_(std::move(parent)), values_({std::move(value)}) { }

NamelessEnv::NamelessEnv (SpNamelessEnv parent, std::vector<Value> values)
    : parent_(std::move(parent)), values_(std::move(values)) { }

SpNamelessEnv NamelessEnv::make_empty () {
  return SpNamelessEnv();
}

SpNamelessEnv NamelessEnv::extend (SpNamelessEnv parent, Value value) {
  return std::make_shared<NamelessEnv>(std::move(parent), std::move(value));
}

SpNamelessEnv NamelessEnv::extend (SpNamelessEnv parent, std::vector<Value> values) {
  return std::make_shared<NamelessEnv>(std::move(parent), std::move(values));
}

Value NamelessEnv::apply (SpNamelessEnv env, LexicalAddr lexicalAddr) {
  size_t n = lexicalAddr.senv_index;
  while (true) {
    if (n != 0 && env) {
      n -= 1;
      env = env->parent_;
    } else {
      break;
    }
  }

  // assert (n == 0 || !env)

  if (n == 0 && env) {
    return env->values_.at(lexicalAddr.entry_index);
  } else { // assert (!env)
    throw SymbolNotFoundError("the specified lexical address does not exist");
  }
}

}