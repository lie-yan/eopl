//
// Created by Lie Yan on 2019-01-31.
//

#include "store.h"

namespace eopl {

SpStore Store::make_empty () {
  return std::make_shared<Store>();
}

SpStore Store::get_store () {
  return nullptr;
}


}

