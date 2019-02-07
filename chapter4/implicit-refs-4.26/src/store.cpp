//
// Created by Lie Yan on 2019-01-31.
//

#include "store.h"

#include "value.h"
#include <gsl/gsl>
#include <utility>

namespace eopl {

Ref Store::newref (Value value) {
  store_.emplace_back(value);
  return Ref{int(store_.size() - 1)};
}

Value Store::deref (Ref value) const {
  return store_[value.location()];
}

Value Store::setref (Ref ref, Value content) {
  store_[ref.location()] = std::move(content);
  return to_value(Int{23});
}

SpStore Store::make_empty () {
  return std::make_shared<Store>();
}

}

