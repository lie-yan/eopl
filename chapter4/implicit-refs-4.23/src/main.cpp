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
var sq, x;
{
  sq = proc (x) (* x x);
  read x;
  (print (sq x));
  read x;
  (print (sq x))
}
)EOF");
}

int main (int argc, char** argv) {

  test_statement_lang();
}