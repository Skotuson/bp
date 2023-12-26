#ifndef TESTSUITE_HPP
#define TESTSUITE_HPP

#include <vector>

class TestSuite {
    public:
        virtual void test ( void ) = 0;
    private:
};

#endif