len([], 0).
len([_|T], L) :- len(T, PL), L is 1 + PL.
