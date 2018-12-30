//
// Created by robin on 2018-12-30.
//

#pragma once

#include <string>
#include <variant>
#include <boost/variant.hpp>

namespace eopl {

using RwString = boost::recursive_wrapper<struct String>;
using RwSymbol = boost::recursive_wrapper<struct Symbol>;
using RwValueList = boost::recursive_wrapper<struct ValueList>;

using Value = std::variant<bool,
                           int,
                           double,
                           RwString,
                           RwSymbol,
                           RwValueList>;

struct String : std::string { using std::string::string; };

struct Symbol : std::string { using std::string::string; };

struct ValueList : std::vector<Value> { using std::vector<Value>::vector; };

}