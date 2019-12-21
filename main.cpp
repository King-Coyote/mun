
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "lua.hpp"
#include "State.hpp"
#include "Ref.hpp"
#include "Table.hpp"
#include "Function.hpp"
#include "StackOps.hpp"
#include "ClassBinding.hpp"

using namespace std;

int main(int argc, char** argv) {

    mun::State s = mun::State();
    auto L = s.getStatePointer();
    s.openLibs();

    s.runString(R"(
        local t = function(number)
            return number + 1
        end
        return t
    )");
    // mun::Function f(s.getStatePointer(), -1);
    // f(10);
    // int num = lua_tointeger(s.getStatePointer(), -1);
}