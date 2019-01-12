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
  EXPECT_EQ(eval(s), Value(Int{5}));

  s = R"EOF(
    let x = 2
    in let y = 3
    in -(x,y)
  )EOF";
  EXPECT_EQ(eval(s), Value(Int{-1}));

  s = R"EOF(
    let x = 3
    in let y = 4
    in *(x,y)
  )EOF";
  EXPECT_EQ(eval(s), Value(Int{12}));

  s = R"EOF(
    let x = 7
    in let y = 3
    in /(x,y)
  )EOF";
  EXPECT_EQ(eval(s), Value(Int{2}));

  s = R"EOF(
    let x = 7
    in let y = 3
    in minus(/(x,y))
  )EOF";
  EXPECT_EQ(eval(s), Value(Int{-2}));
}

TEST(let_lang, relational_operations) {
  using namespace eopl;
  EXPECT_EQ(eval("zero?(0)"), Value(Bool{true}));
  EXPECT_EQ(eval("zero?(-100)"), Value(Bool{false}));

  std::string s = R"EOF(
  let x = 10
  in greater?(x, 12)
  )EOF";
  EXPECT_EQ(eval(s), Value(Bool{false}));

  s = R"EOF(
  let x = 10
  in let y = 100
  in less?(x, y)
  )EOF";
  EXPECT_EQ(eval(s), Value(Bool{true}));

  s = R"EOF(
  let x = 10
  in let z = 100
  in let y = /(z,10)
  in equal?(x, y)
  )EOF";
  EXPECT_EQ(eval(s), Value(Bool{true}));

}

TEST(let_lang, branch_expression) {
  using namespace eopl;

  EXPECT_TRUE(eval(R"EOF(
let z = 5
in let x = 3
in let true = zero?(0)
in let false = zero?(1)
in if zero?(-(z,-(x,-2))) then true else false )EOF") == Value(Bool{true}));

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}