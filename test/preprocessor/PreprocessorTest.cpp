#include "../doctest.h"

#include "../../src/preprocessor/Preprocessor.hpp"

#include <sstream>
#include <iostream>

TEST_CASE("Preprocessor test suite")
{
    Preprocessor p;
    SUBCASE("Natural numbers to successor function representation: ")
    {
        CHECK(Preprocessor::toPeano(0) == "0");
        CHECK(Preprocessor::toPeano(1) == "s(0)");
        CHECK(Preprocessor::toPeano(2) == "s(s(0))");
        CHECK(Preprocessor::toPeano(3) == "s(s(s(0)))");
        CHECK(Preprocessor::toPeano(4) == "s(s(s(s(0))))");
        CHECK(Preprocessor::toPeano(8) == "s(s(s(s(s(s(s(s(0))))))))");
        CHECK(Preprocessor::toPeano(16) == "s(s(s(s(s(s(s(s(s(s(s(s(s(s(s(s(0))))))))))))))))");
    }

    SUBCASE("Peano numbers to natural numbers: ")
    {
        CHECK(Preprocessor::fromPeano("0") == 0);
        CHECK(Preprocessor::fromPeano("s(0)") == 1);
        CHECK(Preprocessor::fromPeano("s(s(s(0)))") == 3);
        CHECK(Preprocessor::fromPeano("s(s(s(s(s(s(s(0)))))))") == 7);
    }
}