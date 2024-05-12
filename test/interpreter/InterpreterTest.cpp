#include "../doctest.h"

#include "../../src/compiler/Compiler.hpp"
#include "../../src/interpreter/Interpreter.hpp"
#include "../../src/desugar/Desugar.hpp"

#include <sstream>
#include <iostream>

TEST_CASE("Interpreter test suite")
{

    auto testQuery = [&](Interpreter &i, Result ref)
    {
        auto r = i.evaluateQuery();
        CHECK(r.first == ref.first);
        CHECK(r.second.size() == ref.second.size());
        for (const auto &res : ref.second)
        {
            // std::cout << r.second[res.first] << " =?= " << res.second << std::endl;
            CHECK(r.second[res.first] == res.second);
        }
    };

    auto nextQuery = [&](Interpreter &i)
    {
        std::istringstream iss = std::istringstream(";");
        i.nextAnswer(iss);
    };

    Compiler c;
    SUBCASE("Run simple queries: ")
    {
        std::istringstream iss(
            "bigger(elephant,mouse)."
            "bigger(mouse,bug)."
            "same(bug,bug).");
        c.compile(iss);
        SUBCASE("Query which should be true:")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bigger(elephant,mouse)."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.empty());
            i.clearQuery();
        }

        SUBCASE("Query which should be false:")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bigger(bug,mouse)."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(!success);
            CHECK(vars.empty());
            i.clearQuery();
        }

        SUBCASE("Query which should be true and bind X to elephant:")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bigger(X,mouse)."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 1);
            CHECK(vars["X"] == "elephant");
            i.clearQuery();
        }

        SUBCASE("Asking for all answers:")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bigger(X,Y)."));
            auto r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "elephant");
            CHECK(r.second["Y"] == "mouse");
            std::istringstream iss(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "mouse");
            CHECK(r.second["Y"] == "bug");
            iss = std::istringstream(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(!r.first);
            CHECK(r.second.empty());
            i.clearQuery();
        }
    }

    SUBCASE("Run queries with recursion: ")
    {
        std::istringstream iss(
            "add(0,Y,Y)."
            "add(s(0),Y,s(Y))."
            "add(s(X),Y,s(Z)):- add(X,Y,Z)."

            "mul(0,X,0)."
            "mul(s(A),B,C) :-"
            "mul(A,B,D),"
            "add(D,B,C)."

            "fact(0,s(0))."
            "fact(s(A),F) :-"
            "fact(A,Y), mul(s(A),Y,F).");
        c.compile(iss);
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "add(s(0),s(0),X)."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 1);
            CHECK(vars["X"] == Desugar::toPeano(2));
            i.clearQuery();
        }

        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "fact(s(s(s(s(s(0))))),X)."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 1);
            CHECK(vars["X"] == Desugar::toPeano(120));
            i.clearQuery();
        }

        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "fact(X,s(s(s(s(s(s(0)))))))."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 1);
            CHECK(vars["X"] == Desugar::toPeano(3));
            i.clearQuery();
        }

        {
            Interpreter i(c.dump());
            std::string q = "fact(X," + Desugar::toPeano(24) + ").";
            i.setQuery(i.compileQuery(q));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 1);
            CHECK(vars["X"] == Desugar::toPeano(4));
            i.clearQuery();
        }

        {
            Interpreter i(c.dump());
            std::string q = "fact(X," + Desugar::toPeano(120) + ").";
            i.setQuery(i.compileQuery(q));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 1);
            CHECK(vars["X"] == Desugar::toPeano(5));
            i.clearQuery();
        }
    }

    SUBCASE("Run queries with recursion II: ")
    {
        std::istringstream iss(
            "bigger(dog, mouse)."
            "bigger(elephant, dog)."
            "bigger(dog, cat)."
            "bigger(whale, elephant)."

            "is_bigger(X, Y) :- bigger(X, Y)."
            "is_bigger(X, Y) :-"
            "bigger(X, Z),"
            "is_bigger(Z, Y).");
        c.compile(iss);

        SUBCASE("Asking for all answers:")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "is_bigger(X,Y)."));
            auto r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "dog");
            CHECK(r.second["Y"] == "mouse");
            std::istringstream iss(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "elephant");
            CHECK(r.second["Y"] == "dog");
            iss = std::istringstream(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "dog");
            CHECK(r.second["Y"] == "cat");
            iss = std::istringstream(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "whale");
            CHECK(r.second["Y"] == "elephant");
            iss = std::istringstream(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "elephant");
            CHECK(r.second["Y"] == "mouse");
            iss = std::istringstream(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "elephant");
            CHECK(r.second["Y"] == "cat");
            iss = std::istringstream(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "whale");
            CHECK(r.second["Y"] == "dog");
            iss = std::istringstream(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "whale");
            CHECK(r.second["Y"] == "mouse");
            iss = std::istringstream(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "whale");
            CHECK(r.second["Y"] == "cat");
            iss = std::istringstream(";");
            i.nextAnswer(iss);

            r = i.evaluateQuery();
            CHECK(!r.first);
            CHECK(r.second.empty());
            i.clearQuery();
        }
    }

    SUBCASE("Run queries with nested complex structures: ")
    {
        std::istringstream iss(
            "t(1,2,s(g(f(1,2)),h(u(o(5)))))."
            "t(s(g(f(1,2)),h(u(o(5)))),x(1))."
            "t(s(g(f(1)),h(u(o(5)))),x(p(2),w(d(1)))).");
        c.compile(iss);

        SUBCASE("Trivial query which should be true:")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "t(s(g(f(1)),h(u(o(5)))),x(p(2),w(d(1))))."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.empty());
            i.clearQuery();
        }

        SUBCASE("Trivial query which should be false:")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "t(s(g(f(2)),h(u(o(5)))),x(p(2),w(d(1))))."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(!success);
            CHECK(vars.empty());
            i.clearQuery();
        }

        SUBCASE("Query with variables binding to complex structures:")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "t(s(X,h(H)),x(Y,Z))."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 4);
            CHECK(vars["X"] == "g(f(1))");
            CHECK(vars["H"] == "u(o(5))");
            CHECK(vars["Y"] == "p(2)");
            CHECK(vars["Z"] == "w(d(1))");
            i.clearQuery();
        }
    }

    SUBCASE("Run queries with recursion III: ")
    {
        std::istringstream iss(
            "bt_identical(empty, empty)."
            "bt_identical(t(_, A, AA), t(_, B, BB)) :-"
            "   bt_identical(A, B), "
            "   bt_identical(AA, BB).");
        c.compile(iss);

        SUBCASE("Trivial satisfiable case")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bt_identical(empty, empty)."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Trivial false case")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bt_identical(non_empty, empty)."));
            testQuery(i, {false, {}});
        }

        SUBCASE("Simple true case")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bt_identical(t(1, empty, empty), t(2, empty, empty))."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Simple false case")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bt_identical(t(1, t(2, empty, empty), empty), t(1, empty, empty))."));
            testQuery(i, {false, {}});
        }
    }

    SUBCASE("Run queries with lists I (append)")
    {
        std::istringstream iss(
            "append([], X, X)."
            "append([H|T1],T2,[H|T3]) :- append(T1, T2, T3).");
        c.compile(iss);

        SUBCASE("Trivial case I (appending empty list to empty list)")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "append([], [], [])."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Trivial case II (appending one element to empty list)")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "append([], [1], [1])."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Trivial satisfiable case")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "append([1,2], [3,4], [1,2,3,4])."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Trivial false case I (output list is longer)")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "append([1,2], [3,4], [1,2,3,4,5])."));
            testQuery(i, {false, {}});
        }

        SUBCASE("Trivial false case II (output list is shorter)")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "append([1,2], [3,4], [1,2,3])."));
            testQuery(i, {false, {}});
        }

        SUBCASE("Variables I")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "append([1,2], [3,4], List)."));
            testQuery(i, {true, {{"List", "[1|[2|[3|[4|[]]]]]"}}});
        }

        SUBCASE("Variables II")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "append([1,2], [3,4], [H|T])."));
            testQuery(i, {true, {{"H", "1"}, {"T", "[2|[3|[4|[]]]]"}}});
        }

        SUBCASE("Variables III")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "append([1,2], [3,4], [W,X,Y,Z])."));
            testQuery(i, {true, {{"W", "1"}, {"X", "2"}, {"Y", "3"}, {"Z", "4"}}});
        }

        SUBCASE("Variables IV")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "append([1,2], X, [1,2,3,4])."));
            testQuery(i, {true, {{"X", "[3|[4|[]]]"}}});
        }

        SUBCASE("Variables V - All answers")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "append(X, Y, [1,2,3])."));
            testQuery(i, {true, {{"X", "[]"}, {"Y", "[1|[2|[3|[]]]]"}}});
            nextQuery(i);
            testQuery(i, {true, {{"X", "[1|[]]"}, {"Y", "[2|[3|[]]]"}}});
            nextQuery(i);
            testQuery(i, {true, {{"X", "[1|[2|[]]]"}, {"Y", "[3|[]]"}}});
            nextQuery(i);
            testQuery(i, {true, {{"X", "[1|[2|[3|[]]]]"}, {"Y", "[]"}}});
            nextQuery(i);
            testQuery(i, {false, {}});
        }
    }

    SUBCASE("Unification operator")
    {
        std::istringstream iss(
            "__id(A,A).");
        c.compile(iss);

        SUBCASE("Const unification I")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "elephant = elephant."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Const unification II")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "elephant = mouse."));
            testQuery(i, {false, {}});
        }

        SUBCASE("Variable unification I")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X=Y."));
            testQuery(i, {true, {{"X", "Y"}}});
        }

        SUBCASE("Variable unification II")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X=Y,Y=Z,Z=elephant."));
            testQuery(i, {true, {{"X", "elephant"}, {"Y", "elephant"}, {"Z", "elephant"}}});
        }

        SUBCASE("Variable unification III")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X=f(g([1,2,s(x)]),t(u(o)))."));
            testQuery(i, {true, {{"X", "f(g([1|[2|[s(x)|[]]]]),t(u(o)))"}}});
        }

        SUBCASE("List unification I")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "[1,2,3]=[1,2,3]."));
            testQuery(i, {true, {}});
        }

        SUBCASE("List unification II")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "[1,2,3]=[1,2,3,4]."));
            testQuery(i, {false, {}});
        }

        SUBCASE("List unification III")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "[]=[]."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Structure unification I")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "f(s(x))=f(s(x))."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Structure unification II")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "f(s(x))=f(s(z))."));
            testQuery(i, {false, {}});
        }
    }

    SUBCASE("Predicates without arguments")
    {
        std::istringstream iss(
            "a."
            "b."
            "c:-a,b.");
        c.compile(iss);

        SUBCASE("Simple query")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "c."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Non-existing fact")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "d."));
            testQuery(i, {false, {}});
        }
    }

    SUBCASE("List decomposition")
    {
        std::istringstream iss(
            "list([1,2]).");
        c.compile(iss);

        SUBCASE("Simple query")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "list([1,2])."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Simple query with decomposition")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "list([1|[2|[]]])."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Simple query with decomposition and variables")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "list([X|Y])."));
            testQuery(i, {true, {{"X", "1"}, {"Y", "[2|[]]"}}});
        }

        SUBCASE("Simple query with decomposition and variables")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "list([X,Y|Z])."));
            testQuery(i, {true, {{"X", "1"}, {"Y", "2"}, {"Z", "[]"}}});
        }

        SUBCASE("Simple query with decomposition and variables - fail case")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "list([X,Y,Z|T])."));
            testQuery(i, {false, {}});
        }
    }

    SUBCASE("List - deletion (no cuts)")
    {
        std::istringstream iss(
            "delete([], _, []).\n"
            "delete([H|T], H, R) :- delete(T, H, R).\n"
            "delete([H|T], E, [H|R]) :- delete(T, E, R).\n");
        c.compile(iss);

        SUBCASE("Simple query")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([], 42, [])."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Delete first element")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3], 1, R)."));
            testQuery(i, {true, {{"R", "[2|[3|[]]]"}}});
        }

        SUBCASE("Delete second element")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3], 2, R)."));
            testQuery(i, {true, {{"R", "[1|[3|[]]]"}}});
        }

        SUBCASE("Delete third element")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3], 3, R)."));
            testQuery(i, {true, {{"R", "[1|[2|[]]]"}}});
        }

        SUBCASE("Delete first and second element")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3], 1, R1),delete(R1, 2, R2)."));
            testQuery(i, {true, {{"R1", "[2|[3|[]]]"}, {"R2", "[3|[]]"}}});
        }

        SUBCASE("Delete multiple occurences")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3,2,3,2,2,2,2,4,9], 2, R)."));
            testQuery(i, {true, {{"R", "[1|[3|[3|[4|[9|[]]]]]]"}}});
        }

        SUBCASE("More answers (no cuts)")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3], 2, R)."));
            testQuery(i, {true, {{"R", "[1|[3|[]]]"}}});
            nextQuery(i);
            testQuery(i, {true, {{"R", "[1|[2|[3|[]]]]"}}});
            nextQuery(i);
            testQuery(i, {false, {}});
        }
    }

    SUBCASE("List - deletion (cuts)")
    {
        std::istringstream iss(
            "delete([], _, []).\n"
            "delete([H|T], H, R) :- delete(T, H, R), !.\n"
            "delete([H|T], E, [H|R]) :- delete(T, E, R).\n");
        c.compile(iss);

        SUBCASE("Simple query")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([], 42, [])."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Delete first element")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3], 1, R)."));
            testQuery(i, {true, {{"R", "[2|[3|[]]]"}}});
        }

        SUBCASE("Delete second element")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3], 2, R)."));
            testQuery(i, {true, {{"R", "[1|[3|[]]]"}}});
        }

        SUBCASE("Delete third element")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3], 3, R)."));
            testQuery(i, {true, {{"R", "[1|[2|[]]]"}}});
        }

        SUBCASE("Delete first and second element")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3], 1, R1),delete(R1, 2, R2)."));
            testQuery(i, {true, {{"R1", "[2|[3|[]]]"}, {"R2", "[3|[]]"}}});
        }

        SUBCASE("Delete multiple occurences")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3,2,3,2,2,2,2,4,9], 2, R)."));
            testQuery(i, {true, {{"R", "[1|[3|[3|[4|[9|[]]]]]]"}}});
        }

        SUBCASE("More answers (cuts)")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "delete([1,2,3], 2, R)."));
            testQuery(i, {true, {{"R", "[1|[3|[]]]"}}});
            nextQuery(i);
            testQuery(i, {false, {}});
        }
    }

    SUBCASE("Cuts")
    {
        std::istringstream iss(
            "a(1).\n"
            "a(2).\n"
            "b(3).\n"
            "b(4).\n"
            "c(5).\n"
            "c(6).\n"
            "d(X,Y,Z):- a(X),!,b(Y),c(Z).");
        c.compile(iss);

        SUBCASE("All possibilities")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "d(X,Y,Z)."));
            testQuery(i, {true, {{"X", "1"}, {"Y", "3"}, {"Z", "5"}}});
            nextQuery(i);
            testQuery(i, {true, {{"X", "1"}, {"Y", "3"}, {"Z", "6"}}});
            nextQuery(i);
            testQuery(i, {true, {{"X", "1"}, {"Y", "4"}, {"Z", "5"}}});
            nextQuery(i);
            testQuery(i, {true, {{"X", "1"}, {"Y", "4"}, {"Z", "6"}}});
            nextQuery(i);
            testQuery(i, {false, {}});
        }
    }

    SUBCASE("Result variable found in the result")
    {
        std::istringstream iss(
            "__id(A,A).");
        c.compile(iss);

        SUBCASE("Simple case")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X=foo(X)."));
            testQuery(i, {true, {{"X", "foo(X)"}}});
        }
    }

    SUBCASE("Query variable found in the result (recursive)")
    {
        std::istringstream iss(
            "__id(A,A).");
        c.compile(iss);

        SUBCASE("Simple case")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X=foo(X)."));
            testQuery(i, {true, {{"X", "foo(X)"}}});
        }

        SUBCASE("Case with two variables")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "Y=X,X=foo(Y)."));
            testQuery(i, {true, {{"X", "foo(X)"}, {"Y", "foo(X)"}}});
        }
    }

    SUBCASE("IS operator tests (REPL)")
    {
        std::istringstream iss(
            "__id(A, A).\n"

            "__add(0,Y,Y)."
            "__add(__s(0),Y,__s(Y))."
            "__add(__s(X),Y,__s(Z)):- __add(X,Y,Z)."

            // x - y = z is the same as y + z = x
            "__sub(X, Y, Z) :- __add(Y, Z, X)."

            "__mul(0,_,0)."
            "__mul(__s(A),B,C) :-"
            "__mul(A,B,D),"
            "__add(D,B,C)."

            // x / y = z is the same as y * z = x
            "__div(X, Y, Z) :- __mul(Y, Z, X).");
        c.compile(iss);

        SUBCASE("Simple case")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "1 is 1."));
            testQuery(i, {true, {}});
        }

        SUBCASE("Simple failing case")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "2 is 1."));
            testQuery(i, {false, {}});
        }

        SUBCASE("Variable assignment I")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X is 1."));
            testQuery(i, {true, {{"X", "1"}}});
        }

        SUBCASE("Variable assignment II")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X is (1 - 1) * 2."));
            testQuery(i, {true, {{"X", "0"}}});
        }

        SUBCASE("Variable assignment III")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X is 2*3, Y is X * 2."));
            testQuery(i, {true, {{"X", "6"}, {"Y", "12"}}});
        }

        SUBCASE("Variable assignment IV")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X = 2, Y is X * 2."));
            testQuery(i, {true, {{"X", "2"}, {"Y", "4"}}});
        }

        SUBCASE("Variable assignment V")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X = 2, Y is X * 2, Z is X + Y."));
            testQuery(i, {true, {{"X", "2"}, {"Y", "4"}, {"Z", "6"}}});
        }

        SUBCASE("Variable assignment VI")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "X is 2-2/2."));
            testQuery(i, {true, {{"X", "1"}}});
        }

        SUBCASE("Variable assignment VII")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "List = [1 + 1,2 * 2, 3 / 3]."));
            testQuery(i, {true, {{"List", "[+(1,1)|[*(2,2)|[/(3,3)|[]]]]"}}});
        }
    }

    SUBCASE("IS operator tests (Program)")
    {
        std::istringstream iss(
            "__id(A, A).\n"

            "__add(0,Y,Y)."
            "__add(__s(0),Y,__s(Y))."
            "__add(__s(X),Y,__s(Z)):- __add(X,Y,Z)."
            // x - y = z is the same as y + z = x
            "__sub(X, Y, Z) :- __add(Y, Z, X)."
            "__mul(0,_,0)."
            "__mul(__s(A),B,C) :-"
            "__mul(A,B,D),"
            "__add(D,B,C)."
            // x / y = z is the same as y * z = x
            "__div(X, Y, Z) :- __mul(Y, Z, X)."
            "decompose([H|T], H, T).");
        c.compile(iss);
        SUBCASE("Variable assignment")
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "List = [1 + 1,2 * 2, 3 / 3], decompose(List, H, T), X is H."));
            testQuery(i, {true, {{"List", "[+(1,1)|[*(2,2)|[/(3,3)|[]]]]"}, {"H", "+(1,1)"}, {"T", "[*(2,2)|[/(3,3)|[]]]"}, {"X", "+(1,1)"}}});
        }
    }
}