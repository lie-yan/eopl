//
// Created by robin on 2019-01-13.
//

#include <gtest/gtest.h>
#include "nameless/interpreter.h"

TEST(let_lang, arithmetic_operations) {
  using namespace eopl;
  std::string s = R"EOF(
    let x = 2
    in let y = 3
    in (+ x y)
  )EOF";
  EXPECT_EQ(nameless_eval(s), to_value(Int{5}));

  s = R"EOF(
    let x = 2
    in let y = 3
    in (- x y)
  )EOF";
  EXPECT_EQ(nameless_eval(s), to_value(Int{-1}));

  s = R"EOF(
    let x = 3
    in let y = 4
    in (* x y)
  )EOF";
  EXPECT_EQ(nameless_eval(s), to_value(Int{12}));

  s = R"EOF(
    let x = 7
    in let y = 3
    in (/ x y)
  )EOF";
  EXPECT_EQ(nameless_eval(s), to_value(Int{2}));

  s = R"EOF(
    let x = 7
    in let y = 3
    in (minus (/ x y))
  )EOF";
  EXPECT_EQ(nameless_eval(s), to_value(Int{-2}));
}

TEST(let_lang, relational_operations) {
  using namespace eopl;
  EXPECT_EQ(nameless_eval("(zero? 0)"), to_value(Bool{true}));
  EXPECT_EQ(nameless_eval("(zero? -100)"), to_value(Bool{false}));

  std::string s = R"EOF(
  let x = 10
  in (greater? x 12)
  )EOF";
  EXPECT_EQ(nameless_eval(s), to_value(Bool{false}));

  s = R"EOF(
  let x = 10
  in let y = 100
  in (less? x y)
  )EOF";
  EXPECT_EQ(nameless_eval(s), to_value(Bool{true}));

  s = R"EOF(
  let x = 10
  in let z = 100
  in let y = (/ z 10)
  in (equal? x y)
  )EOF";
  EXPECT_EQ(nameless_eval(s), to_value(Bool{true}));

}

TEST(let_lang, branch_expression) {
  using namespace eopl;

  EXPECT_TRUE(nameless_eval(R"EOF(
let z = 5
in let x = 3
in let true = (zero? 0)
in let false = (zero? 1)
in if (zero? (- z (- x -2))) then true else false )EOF") == to_value(Bool{true}));

}

TEST(let_lang, list) {
  using namespace eopl;

  {
    std::ostringstream ss;
    auto res = nameless_eval(R"EOF(
let x = 4 in (cons x
(cons (cons (- x 1) emptylist) emptylist))
)EOF");
    ss << res;
    EXPECT_EQ(ss.str(), "(4 (3))");
  }

  {
    std::ostringstream ss;
    auto res = nameless_eval("let x = 4 in (list x (- x 1) (- x 3))");
    ss << res;
    EXPECT_EQ(ss.str(), "(4 3 1)");
  }
}

TEST(let_lang, cond) {
  using namespace eopl;

  auto res = nameless_eval(R"EOF(
let x = 10
in cond (zero? x) ==> (+ x 1)
        (greater? x 3) ==> (+ x 2)
        (less? x 100) ==> (+ x 3)
   end
)EOF");

  EXPECT_EQ(res, to_value(Int(12)));

  EXPECT_THROW(
      nameless_eval("cond (zero? 1) ==> 10 "
                    "    (greater? 10 11) ==> 20"
                    "    (less? 20 19) ==> 30"
                    "end"),
      std::runtime_error
  );

  EXPECT_THROW(
      nameless_eval("let x = 10 in cond end"),
      std::runtime_error
  );
}

TEST(let_lang, let) {
  using namespace eopl;

  EXPECT_EQ(nameless_eval("let x = 30"
                          "in let x = (- x 1)"
                          "       y = (- x 2)"
                          "in (- x y)"), to_value(Int{1}));

  EXPECT_EQ(nameless_eval("let x = 30"
                          "in let* x = (- x 1)"
                          "        y = (- x 2)"
                          "in (- x y)"), to_value(Int{2}));

}

TEST(let_lang, unpack) {
  using namespace eopl;

  EXPECT_EQ(nameless_eval("let u = 7 in unpack x y = (cons u (cons 3 emptylist)) in (- x y)"), to_value(Int{4}));
  EXPECT_THROW(
      nameless_eval("let u = 7 in unpack x y z = (cons u (cons 3 emptylist)) in (- x y)"),
      std::runtime_error
  );
}

TEST(proc_lang, basic) {
  using namespace eopl;

  EXPECT_EQ(nameless_eval("let f = proc (x) (- x 11) in (f (f 77))"),
            to_value(Int{55}));
  EXPECT_EQ(nameless_eval("(proc (f) (f (f 77)) proc (x) (- x 11))"),
            to_value(Int{55}));
}

TEST(proc_lang, multi_param) {
  using namespace eopl;

  EXPECT_EQ(nameless_eval("let sq = proc (x) (* x x)"
                          "in let sum = proc (x, y) (+ x y) "
                          "in (sum (sq 3) (sq 4))"),
            to_value(Int{25}));
}

TEST(letrec_lang, basic) {
  using namespace eopl;

  EXPECT_EQ(
      nameless_eval("letrec double(x) = if (zero? x) then 0 else (- (double (- x 1)) -2) "
           "in (double 6)"),
      to_value(Int{12}));
}

TEST(letrec_lang, mutual_recursive) {
  using namespace eopl;

  EXPECT_EQ(
      nameless_eval("letrec "
           "even(x) = if (zero? x) then 1 else (odd (- x 1)) "
           "odd(x) = if (zero? x) then 0 else (even (- x 1)) "
           "in let even = proc(x) (* x x) "
           "in (odd 13)"),
      to_value(Int{1}));
}

int main (int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}