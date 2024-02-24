add(0,Y,Y).
add(s(0),Y,s(Y)).
add(s(X),Y,s(Z)):- add(X,Y,Z).