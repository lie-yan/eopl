//
// Created by Lie Yan on 2019-01-31.
//

#pragma once

#include <memory>

namespace eopl {

using SpStore = std::shared_ptr<struct Store>;

struct Store {
  static SpStore make_empty ();
  static SpStore get_store ();
};

}