#include "../doctest.h"

#include "../../src/compiler/Compiler.hpp"
#include "../../src/interpreter/Interpreter.hpp"
#include "../../src/preprocessor/Preprocessor.hpp"

#include <sstream>
#include <iostream>

TEST_CASE("Interpreter test suite")
{
    Compiler c;
    SUBCASE("Run simple queries: ")
    {
        std::istringstream iss(
            "bigger(elephant,mouse)."
            "bigger(mouse,bug)."
            "same(bug,bug).");
        c.compile(iss);
        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bigger(elephant,mouse)."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.empty());
            i.clearQuery();
        }

        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bigger(bug,mouse)."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(!success);
            CHECK(vars.empty());
            i.clearQuery();
        }

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

        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bigger(X,Y)."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 2);
            CHECK(vars["X"] == "elephant");
            CHECK(vars["Y"] == "mouse");
            i.clearQuery();
        }

        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "bigger(X,Y)."));
            auto r = i.evaluateQuery();
            CHECK(r.first);
            CHECK(r.second.size() == 2);
            CHECK(r.second["X"] == "elephant");
            CHECK(r.second["Y"] == "mouse");
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
            CHECK(vars["X"] == Preprocessor::toPeano(2));
            i.clearQuery();
        }

        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "fact(s(s(s(s(s(0))))),X)."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 1);
            CHECK(vars["X"] == Preprocessor::toPeano(120));
            i.clearQuery();
        }

        {
            Interpreter i(c.dump());
            i.setQuery(i.compileQuery(
                "fact(X,s(s(s(s(s(s(0)))))))."));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 1);
            CHECK(vars["X"] == Preprocessor::toPeano(3));
            i.clearQuery();
        }

        {
            Interpreter i(c.dump());
            std::string q = "fact(X," + Preprocessor::toPeano(24) + ").";
            i.setQuery(i.compileQuery(q));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 1);
            CHECK(vars["X"] == Preprocessor::toPeano(4));
            i.clearQuery();
        }

        {
            Interpreter i(c.dump());
            std::string q = "fact(X," + Preprocessor::toPeano(120) + ").";
            i.setQuery(i.compileQuery(q));
            auto [success, vars] = i.evaluateQuery();
            CHECK(success);
            CHECK(vars.size() == 1);
            CHECK(vars["X"] == Preprocessor::toPeano(5));
            i.clearQuery();
        }
    }

    SUBCASE("Run queries with lists (append): ")
    {
        std::istringstream iss(
            "append([], X, X)."
            "append([H|T1],T2,[H|T3]) :- append(T1, T2, T3).");
        c.compile(iss);
    }
}