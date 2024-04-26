a(a).
a(b).
b(b).

e(a).
e(b).
f(b).

c(X):-a(X),!,b(X),e(Y),f(Y).
d(X):-a(X),b(X).
g(X):-a(X),!,e(Y),f(Y).