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
            std::cout << r.second[res.first] << " =?= " << res.second << std::endl;
            CHECK(r.second[res.first] == res.second);
        }
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
    }
}