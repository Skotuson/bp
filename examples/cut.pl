a(a).
a(b).
b(b).
c(X):-a(X),!,b(X).
d(X):-a(X),b(X).