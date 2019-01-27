//
// Created by Lie Yan on 2019-01-27.
//

#pragma once

#include <memory>
#include <value_fwd.h>
#include "senv.h"

namespace eopl {

using SpNamelessEnv = std::shared_ptr<struct NamelessEnv>;

struct NamelessEnv {

  NamelessEnv () = default;
  NamelessEnv (const NamelessEnv&) = delete;
  NamelessEnv (NamelessEnv&&) = delete;
  NamelessEnv& operator = (const NamelessEnv&) = delete;
  NamelessEnv& operator = (NamelessEnv&&) = delete;

  NamelessEnv (SpNamelessEnv parent, Value value);
  NamelessEnv (SpNamelessEnv parent, std::vector<Value> values);

  static SpNamelessEnv make_empty ();
  static SpNamelessEnv extend (SpNamelessEnv parent, Value value);
  static SpNamelessEnv extend (SpNamelessEnv parent, std::vector<Value> values);
  static Value apply (SpNamelessEnv env, LexicalAddr lexicalAddr);

private:
  SpNamelessEnv parent_;
  std::vector<Value> values_;
};


}