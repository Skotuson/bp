list([1,2,3]).
append([], X, X).
append([H|T1],T2,[H|T3]) :- append(T1, T2, T3).