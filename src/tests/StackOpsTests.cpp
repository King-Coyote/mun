#include "catch.hpp"
#include "State.hpp"
#include "StackOps.hpp"

#include <memory>
#include <iostream>

using namespace std;
using namespace mun;

SCENARIO("mun::StackOps functions work as expected", "[stackops]") {

    GIVEN("A mun::Function object") {

        auto p_State = unique_ptr<State>(new State());
        auto L = p_State->getStatePointer();
        p_State->openLibs();

        WHEN("Items have been placed on the stack") {
            THEN("The stack count is correct") {
                pushValue(L, 10);
                pushValue(L, 10);
                pushValue(L, 10);
                CHECK(countStack(L) == 3);
                lua_pop(L, -1);
                CHECK(countStack(L) == 0);
            }
        }

    }

}