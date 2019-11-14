//
// Created by robin on 2019-01-13.
//

#include <gtest/gtest.h>
#include "type/checker.h"

TEST(let_lang, arithmetic_operations) {
  using namespace eopl;
  std::string s = R"EOF(
    let x = 2
    in let y = 3
    in (+ x y)
  )EOF";
  EXPECT_NO_THROW(type::check_program(s));

  s = R"EOF(
    let x = 2
    in let y = 3
    in (- x y)
  )EOF";
  EXPECT_NO_THROW(type::check_program(s));

  s = R"EOF(
    let x = 3
    in let y = 4
    in (* x y)
  )EOF";
  EXPECT_NO_THROW(type::check_program(s));

  s = R"EOF(
    let x = 7
    in let y = 3
    in (/ x y)
  )EOF";
  EXPECT_NO_THROW(type::check_program(s));

  s = R"EOF(
    let x = 7
    in let y = 3
    in (minus (/ x y))
  )EOF";
  EXPECT_NO_THROW(type::check_program(s));
}

TEST(let_lang, relational_operations) {
  using namespace eopl;
  EXPECT_NO_THROW(type::check_program("(zero? 0)"));
  EXPECT_NO_THROW(type::check_program("(zero? -100)"));

  std::string s = R"EOF(
  let x = 10
  in (greater? x 12)
  )EOF";
  EXPECT_NO_THROW(type::check_program(s));

  s = R"EOF(
  let x = 10
  in let y = 100
  in (less? x y)
  )EOF";
  EXPECT_NO_THROW(type::check_program(s));

  s = R"EOF(
  let x = 10
  in let z = 100
  in let y = (/ z 10)
  in (equal? x y)
  )EOF";
  EXPECT_NO_THROW(type::check_program(s));

}

TEST(let_lang, branch_expression) {
  using namespace eopl;

  EXPECT_NO_THROW(type::check_program(R"EOF(
let z = 5
in let x = 3
in let true = (zero? 0)
in let false = (zero? 1)
in if (zero? (- z (- x -2))) then true else false )EOF"));

}

//TEST(let_lang, list) {
//  using namespace eopl;
//
//  {
//    type::check_program(R"EOF(
//let x = 4 in (cons x
//(cons (cons (- x 1) emptylist) emptylist))
//)EOF");
//  }
//
//  {
//    type::check_program("let x = 4 in (list x (- x 1) (- x 3))");
//  }
//}

TEST(let_lang, cond) {
  using namespace eopl;

  type::check_program(R"EOF(
let x = 10
in cond (zero? x) ==> (+ x 1)
        (greater? x 3) ==> (+ x 2)
        (less? x 100) ==> (+ x 3)
   end
)EOF");

  EXPECT_NO_THROW(
      type::check_program("cond (zero? 1) ==> 10 "
                          "    (greater? 10 11) ==> 20"
                          "    (less? 20 19) ==> 30"
                          "end"));

  EXPECT_THROW(
      type::check_program("let x = 10 in cond end"),
      std::runtime_error
  );
}

TEST(let_lang, let) {
  using namespace eopl;

  EXPECT_NO_THROW(type::check_program("let x = 30"
                                      "in let x = (- x 1)"
                                      "       y = (- x 2)"
                                      "in (- x y)"));

  EXPECT_NO_THROW(type::check_program("let x = 30"
                                      "in let* x = (- x 1)"
                                      "        y = (- x 2)"
                                      "in (- x y)"));

}

//TEST(let_lang, unpack) {
//  using namespace eopl;
//
//  EXPECT_NO_THROW(type::check_program("let u = 7 in unpack x y = (cons u (cons 3 emptylist)) in (- x y)"));
//  EXPECT_THROW(
//      type::check_program("let u = 7 in unpack x y z = (cons u (cons 3 emptylist)) in (- x y)"),
//      std::runtime_error
//  );
//}

TEST(proc_lang, basic) {
  using namespace eopl;

  EXPECT_NO_THROW(type::check_program("let f = proc (x : int) (- x 11) in (f (f 77))"));
  EXPECT_NO_THROW(type::check_program("let f = proc (x : int) (- x 11) in f"));
  EXPECT_NO_THROW(type::check_program("(proc (f : (int -> int)) (f (f 77)) proc (x : int) (- x 11))"));
}

TEST(proc_lang, multi_param) {
  using namespace eopl;

  EXPECT_NO_THROW(type::check_program("let sq = proc (x : int) (* x x)"
                                      "in let sum = proc (x : int, y : int) (+ x y) "
                                      "in (sum (sq 3) (sq 4))"));
}

TEST(letrec_lang, basic) {
  using namespace eopl;

  EXPECT_NO_THROW(
      type::check_program("letrec int double(x : int) = if (zero? x) then 0 else (- (double (- x 1)) -2) "
                          "in (double 6)"));
}

TEST(letrec_lang, mutual_recursive) {
  using namespace eopl;

  EXPECT_NO_THROW(
      type::check_program("letrec "
                          "int even(x : int) = if (zero? x) then 1 else (odd (- x 1)) "
                          "int odd(x : int) = if (zero? x) then 0 else (even (- x 1)) "
                          "in let even = proc(x : int) (* x x) "
                          "in (odd 13)"));
}

TEST(checked_lang, pair) {
  using namespace eopl::type;

  EXPECT_NO_THROW(
      check_program(
          "let* x = 1 "
          "     y = 2 "
          "     z = newpair(x, (+ x y)) "
          "in z"));

  EXPECT_NO_THROW(
      check_program(
          "let* x = 1 "
          "     y = 2 "
          "     z = newpair(x, (+ x y)) "
          "in unpair a b = z "
          "in (- a 10)"));

}

int main (int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}