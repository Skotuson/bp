fact(0, 1).
fact(N, Res) :-
    N1 is N - 1,
    fact(N1, Tmp), 
    Res is Tmp * N.