a(Y) :- X = foo(X), X = Y. //foo(Y)
b(X) :- X = Y, X = foo(X).
c(X,Y) :- Y is X + 1.

eval(+(A,B), R) :- R is A + B

A is 2 + 2 //A = 4
_a1 = A
_a2 = 2
_a3 = 2
add(_a2,_a3, _a4)
_a1 = a_4

A is 2 + B, B = 2
_a1 = A
_a2 = 2
_a3 = 2
add(_a2,_a3, _a4)
_a1 = a_4