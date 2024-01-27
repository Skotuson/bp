add(Y,Y).
add(s(X),Y,s(Y)).
add(s(X),Y,s(Z)):- add(X,Y,Z).