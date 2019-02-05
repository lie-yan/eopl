//
// Created by Lie Yan on 2019-01-27.
//

#pragma once

#include <stdexcept>

namespace eopl {

struct SymbolNotFoundError : std::runtime_error {
  using runtime_error::runtime_error;
};

}