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

int main (int argc, char** argv) {

  test_statement_lang();
}