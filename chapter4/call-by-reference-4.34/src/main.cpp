//
// Created by robin on 2019-01-13.
//

#include "interpreter.h"

void test_statement_lang () {
  eopl::run(R"EOF(
var x, y;
{
x = 3;
y = 4;
(print (+ x y))
}
)EOF");

  eopl::run(R"EOF(
var x,y,z;
{
x = 3; % Example 2
y = 4;
z = 0;
while (not (zero? x))
{
  z = (+ z y);
  x = (- x 1)
};
(print z)
}
)EOF");

  eopl::run(R"EOF(
var x;
{
  x = 3; % Example 3
  (print x);
  var x;
  {
    x = 4;
    (print x)
  };
  (print x)
}
)EOF");

  eopl::run(R"EOF(
var f,x;
{
  f = proc(x,y) (* x y); % Example 4
  x = 3;
  (print (f 4 x))
}
)EOF");

  eopl::run(R"EOF(
var x;
{
  x = 4;
  do
  {
    (print x);
    x = (- x 1)
  } while (not (zero? x))
}
)EOF");

  eopl::run(R"EOF(
var x = (zero? 1), y = (* 3 3);
{
  (print x);
  (print y)
}
)EOF");

  eopl::run(R"EOF(
var odd = proc (x) if (zero? x) then 0 else (even (- x 1)),
    even = proc (x) if (zero? x) then 1 else (odd (- x 1));
{
  (print (odd 13));
  (print (even 13))
}
)EOF");

  eopl::run(R"EOF(
var print_n = subr (x) while (not (zero? x)) { (print x); x = (- x 1) };
{
  (print 100);
  (print_n 3);
  (print_n 5);
  (print 101)
}
)EOF");
}

void test_call_by_reference () {
  eopl::run(R"EOF(
var x;
{
  x = let p = proc (x) set x = 4
      in let a = 3
        in begin (p a); a end;
  (print x)
}
)EOF");

  eopl::run(R"EOF(
var x;
{
  x = let f = proc (x) set x = 44
    in let g = proc (y) (f y)
    in let z = 55
    in begin (g z); z end;
  (print x)
}
)EOF");

  eopl::run(R"EOF(
var swap, a, b;
{
  swap = subr (x, y)
         var temp;
         {
           temp = x;
           x = y;
           y = temp
         };
  a = 33;
  b = 44;
  (print a);
  (print b);
  (swap a b);
  (print a);
  (print b)
}
)EOF");

  eopl::run(R"EOF(
var a = 44, b = 33;
{
  (print letref x = a in begin set x = (+ x 1) end);
  (print let x = b in begin set x = (+ x 1) end);
  (print a);
  (print b)
}
)EOF");
}

int main (int argc, char** argv) {

  test_call_by_reference();
}