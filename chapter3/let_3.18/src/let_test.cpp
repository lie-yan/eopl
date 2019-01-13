//
// Created by robin on 2019-01-13.
//

#include <gtest/gtest.h>
#include "interpreter.h"

TEST(let_lang, arithmetic_operations) {
  using namespace eopl;
  std::string s = R"EOF(
    let x = 2
    in let y = 3
    in +(x,y)
  )EOF";
  EXPECT_EQ(eval(s), int_to_value(Int{5}));

  s = R"EOF(
    let x = 2
    in let y = 3
    in -(x,y)
  )EOF";
  EXPECT_EQ(eval(s), int_to_value(Int{-1}));

  s = R"EOF(
    let x = 3
    in let y = 4
    in *(x,y)
  )EOF";
  EXPECT_EQ(eval(s), int_to_value(Int{12}));

  s = R"EOF(
    let x = 7
    in let y = 3
    in /(x,y)
  )EOF";
  EXPECT_EQ(eval(s), int_to_value(Int{2}));

  s = R"EOF(
    let x = 7
    in let y = 3
    in minus(/(x,y))
  )EOF";
  EXPECT_EQ(eval(s), int_to_value(Int{-2}));
}

TEST(let_lang, relational_operations) {
  using namespace eopl;
  EXPECT_EQ(eval("zero?(0)"), bool_to_value(Bool{true}));
  EXPECT_EQ(eval("zero?(-100)"), bool_to_value(Bool{false}));

  std::string s = R"EOF(
  let x = 10
  in greater?(x, 12)
  )EOF";
  EXPECT_EQ(eval(s), bool_to_value(Bool{false}));

  s = R"EOF(
  let x = 10
  in let y = 100
  in less?(x, y)
  )EOF";
  EXPECT_EQ(eval(s), bool_to_value(Bool{true}));

  s = R"EOF(
  let x = 10
  in let z = 100
  in let y = /(z,10)
  in equal?(x, y)
  )EOF";
  EXPECT_EQ(eval(s), bool_to_value(Bool{true}));

}

TEST(let_lang, branch_expression) {
  using namespace eopl;

  EXPECT_TRUE(eval(R"EOF(
let z = 5
in let x = 3
in let true = zero?(0)
in let false = zero?(1)
in if zero?(-(z,-(x,-2))) then true else false )EOF") == bool_to_value(Bool{true}));

}

TEST(let_lang, list) {
  using namespace eopl;

  {
    std::ostringstream ss;
    auto res = eval(R"EOF(
let x = 4 in cons(x,
cons(cons(-(x,1), emptylist), emptylist))
)EOF");
    ss << res;
    EXPECT_EQ(ss.str(), "(4 (3))");
  }

  {
    std::ostringstream ss;
    auto res = eval("let x = 4 in list(x, -(x,1), -(x,3))");
    ss << res;
    EXPECT_EQ(ss.str(), "(4 3 1)");
  }
}

TEST(let_lang, cond) {
  using namespace eopl;

  auto res = eval(R"EOF(
let x = 10
in cond zero?(x) ==> +(x,1)
        greater?(x, 3) ==> +(x,2)
        less?(x, 100) ==> +(x,3)
   end
)EOF");

  EXPECT_EQ(res, int_to_value(Int(12)));

  EXPECT_THROW(
      eval("cond zero?(1) ==> 10 "
           "    greater?(10, 11) ==> 20"
           "    less?(20, 19) ==> 30"
           "end"),
      std::runtime_error
  );

  EXPECT_THROW(
      eval("let x = 10 in cond end"),
      std::runtime_error
  );
}

TEST(let_lang, let) {
  using namespace eopl;

  EXPECT_EQ(eval("let x = 30"
                 "in let x = -(x,1)"
                 "       y = -(x,2)"
                 "in -(x,y)"), int_to_value(Int{1}));

  EXPECT_EQ(eval("let x = 30"
                 "in let* x = -(x,1)"
                 "        y = -(x,2)"
                 "in -(x,y)"), int_to_value(Int{2}));

}

int main (int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}