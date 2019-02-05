//
// Created by Lie Yan on 2019-01-31.
//

#pragma once

#include <memory>
#include "value_fwd.h"

namespace eopl {

using SpStore = std::shared_ptr<struct Store>;

class Store {
public:

  static SpStore make_empty ();
  Ref newref (Value value);
  Value deref (Ref value) const;
  Value setref (Ref ref, Value content);
private:
  std::vector<Value> store_;
};

}