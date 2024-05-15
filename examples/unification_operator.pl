eats(49).
eats(pancakes).
eats(olives).
eats(dish(pancakes,syrup)).
eats(dish(olives,martini)).
jeff:-eats(X),X=olives.
martha:-eats(X),X=49.
alice:-eats(X),X=dish(pancakes, syrup).
bob:-eats(X),X=Y,Y=dish(olives,martini).
nonsensor:-dish(olives,martini)=dish(vodka,soda).