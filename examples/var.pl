bigger(dog, mouse).
bigger(elephant, dog).
bigger(dog, cat).
bigger(whale, elephant).

id(A,A).

is_bigger(X, Y) :- bigger(X, Y).
is_bigger(X, Y) :-
    bigger(X, Z),
    is_bigger(Z, Y).