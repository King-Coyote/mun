#include "catch.hpp"
#include "Function.hpp"
#include "State.hpp"

#include <memory>
#include <iostream>
#include <functional>

using namespace std;
using namespace mun;

#define TEST_VALID_FUNC_PATH "/home/alex/projects/mun/src/tests/validFunction.lua"
#define TEST_INVALID_FUNC_PATH "/home/alex/projects/mun/src/tests/invalidFunction.lua"

SCENARIO("mun::Function works as expected", "[function]") {

    GIVEN("A mun::Function object") {

        auto p_State = unique_ptr<State>(new State());
        auto L = p_State->getStatePointer();
        p_State->runScript(TEST_VALID_FUNC_PATH);
        auto func = Function(L, -1);
        lua_pop(L, -1);
        p_State->openLibs();

        WHEN("A function is created") {
            THEN("its construction is valid and throws no errors and leaves the stack empty") {
                CHECK(func.getHandle() != LUA_NOREF);
            }
        }

        WHEN("A valid function from a file is invoked correctly") {
            THEN("It correctly runs") {
                ErrorCallback callback = [&](const string& s) {};
                CHECK_NOTHROW(func(callback, 0));
            }

            THEN("It correctly returns a value") {
                ErrorCallback callback = nullptr;
                int ret = func.call<int>(callback, 10);
                CHECK(ret == 11);
            }
        }

        WHEN("An invalid function from a file is invoked") {
            p_State->runScript(TEST_INVALID_FUNC_PATH);
            auto invalidFunc = Function(L, -1);
            lua_pop(L, 1);
            CHECK(countStack(L) == 0);
            THEN("It invokes the provided callback") {
                int incrementMe = 0;
                ErrorCallback callback = [&](const string& s) {
                    incrementMe++;
                };
                CHECK_NOTHROW(invalidFunc(callback, 0));
                CHECK(incrementMe == 1);
            }
        }

    }

}