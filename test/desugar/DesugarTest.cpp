#include "../doctest.h"

#include "../../src/desugar/Desugar.hpp"

#include <sstream>
#include <iostream>

TEST_CASE("Desugar test suite")
{
    SUBCASE("Natural numbers to successor function representation: ")
    {
        CHECK(Desugar::toPeano(0) == "0");
        CHECK(Desugar::toPeano(1) == "s(0)");
        CHECK(Desugar::toPeano(2) == "s(s(0))");
        CHECK(Desugar::toPeano(3) == "s(s(s(0)))");
        CHECK(Desugar::toPeano(4) == "s(s(s(s(0))))");
        CHECK(Desugar::toPeano(8) == "s(s(s(s(s(s(s(s(0))))))))");
        CHECK(Desugar::toPeano(16) == "s(s(s(s(s(s(s(s(s(s(s(s(s(s(s(s(0))))))))))))))))");
    }

    SUBCASE("Peano numbers to natural numbers: ")
    {
        CHECK(Desugar::fromPeano("0") == 0);
        CHECK(Desugar::fromPeano("s(0)") == 1);
        CHECK(Desugar::fromPeano("s(s(s(0)))") == 3);
        CHECK(Desugar::fromPeano("s(s(s(s(s(s(s(0)))))))") == 7);
    }

    SUBCASE("Natural numbers to successor function representation (underscore): ")
    {
        CHECK(Desugar::toPeano(0, true) == "0");
        CHECK(Desugar::toPeano(1, true) == "__s(0)");
        CHECK(Desugar::toPeano(2, true) == "__s(__s(0))");
        CHECK(Desugar::toPeano(3, true) == "__s(__s(__s(0)))");
        CHECK(Desugar::toPeano(4, true) == "__s(__s(__s(__s(0))))");
        CHECK(Desugar::toPeano(8, true) == "__s(__s(__s(__s(__s(__s(__s(__s(0))))))))");
    }

    SUBCASE("Peano numbers to natural numbers: ")
    {
        CHECK(Desugar::fromPeano("0", true) == 0);
        CHECK(Desugar::fromPeano("__s(0)", true) == 1);
        CHECK(Desugar::fromPeano("__s(__s(__s(0)))", true) == 3);
        CHECK(Desugar::fromPeano("__s(__s(__s(__s(__s(__s(__s(0)))))))", true) == 7);
    }

    SUBCASE("Peano number replacing with natural numbers: ")
    {
        CHECK(Desugar::replacePeano("0") == "0");
        CHECK(Desugar::replacePeano("__s(0)") == "1");
        CHECK(Desugar::replacePeano("__s(__s(0))") == "2");
        CHECK(Desugar::replacePeano("f(__s(0),__s(__s(0)))") == "f(1,2)");
        CHECK(Desugar::replacePeano("[__s(0)|[__s(__s(0))|[__s(__s(__s(0)))|[]]]]") == "[1|[2|[3|[]]]]");
    }
}