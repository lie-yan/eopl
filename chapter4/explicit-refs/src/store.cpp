//
// Created by Lie Yan on 2019-01-31.
//

#include "store.h"

#include "value.h"
#include <gsl/gsl>
#include <utility>

namespace eopl {

Value Store::newref (Value value) {
  store_.emplace_back(value);
  return to_value(Ref{int(store_.size() - 1)});
}

Value Store::deref (Value value) const {
  Expects(type_of(value) == ValueType::REF);
  return store_[to_ref(value).location()];
}

Value Store::setref (Value ref, Value content) {
  Expects(type_of(ref) == ValueType::REF);

  store_[to_ref(ref).location()] = std::move(content);
  return to_value(Int{23});
}

SpStore Store::make_empty () {
  return std::make_shared<Store>();
}

}

