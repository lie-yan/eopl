//
// Created by robin on 2018-12-30.
//

#pragma once

#include <string>
#include <variant>
#include <boost/variant.hpp>

namespace eopl {

struct Symbol : std::string { using std::string::string; };
struct String : std::string { using std::string::string; };

using Value = std::variant<bool,
                           int,
                           double,
                           boost::recursive_wrapper<String>,
                           boost::recursive_wrapper<Symbol>,
                           boost::recursive_wrapper<struct ValueList>>;

struct ValueList : std::vector<Value> { };

}