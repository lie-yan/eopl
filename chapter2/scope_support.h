//
// Created by robin on 2018-12-30.
//

#pragma once

#include <string>
#include <variant>
#include <boost/variant.hpp>

namespace eopl {

using Variable = std::string;

using Value = std::variant<bool,
                           int,
                           double,
                           boost::recursive_wrapper<std::string>,
                           boost::recursive_wrapper<struct ValueList>>;

struct ValueList : std::vector<Value> { };

}