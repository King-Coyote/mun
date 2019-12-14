#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "State.hpp"

#include <memory>
#include <iostream>

using namespace std;
using namespace mun;

#define TEST_INVALID_SCRIPT_PATH "/home/alex/projects/mun/src/tests/invalidScript.lua"

namespace {
class DummyStateTester {
public:
    int callbackCount = 0;

    void callback(const string& msg) {
        cout << msg << endl;
        this->callbackCount++;
    }
};
}

SCENARIO("State works as expected", "[state]") {

    GIVEN("A mun::State object") {

        auto p_State = unique_ptr<State>(new State());
        p_State->openLibs();

        WHEN("A state object is constructed") {
            THEN("The construction works as expected") {
                CHECK(p_State.get() != nullptr);
            }
        }

        WHEN("An invalid script file is run") {
            THEN("The state correctly uses a lambda error callback") {
                int count = 0;
                auto lambda = [&](const string& msg) -> void {
                    cout << msg << endl;
                    count++;
                };
                p_State->runScript(TEST_INVALID_SCRIPT_PATH, lambda);
                p_State->runScript(TEST_INVALID_SCRIPT_PATH, lambda);
                CHECK(count == 2);
            }
        }

        WHEN("An invalid script file is run") {
            THEN("The state correctly uses a class callback") {
                auto p_Dummy = unique_ptr<DummyStateTester>(new DummyStateTester());
                auto lambdaCaller = [&](const string& msg) -> void {
                    p_Dummy->callback(msg);
                };
                p_State->runScript(TEST_INVALID_SCRIPT_PATH, lambdaCaller);
                p_State->runScript(TEST_INVALID_SCRIPT_PATH, lambdaCaller);
                CHECK(p_Dummy->callbackCount == 2);
            }
        }

    }

}