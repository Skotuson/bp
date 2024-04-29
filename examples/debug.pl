add(0,Y,Y).
add(s(0),Y,s(Y)).
add(s(X),Y,s(Z)):- add(X,Y,Z).

mul(0,X,0).
mul(s(A),B,C) :-
    mul(A,B,D),
    add(D,B,C).