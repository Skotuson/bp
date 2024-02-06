qsort([], Right, Right).

qsort([X|Left], Right0, Right):- partition(Left, X, Left1, Left2),
    qsort(Left1,Right0,[X|Right1]),
    qsort(Left2,Right1,Right).

partition([],_,[],[]).

partition([X|Left],Pivot,[X|Left1],Left2) :- 
    parititon(Left, Pivot, Left1, Left2).

partition([X|Left],Pivot,Left1,[X|Left2]):- 
    partition(Left, Pivot, Left1, Left2).