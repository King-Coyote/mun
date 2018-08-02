#pragma once

#include <iostream>
#include <string>
#include "lua.hpp"
#include "Ref.hpp"
#include "StackOps.hpp"

namespace mun {
    class Function;
}

class mun::Function : public Ref {
public:
    Function() : Ref() {}
    Function(lua_State* L, int index = -1) : Ref(L, index) {}
    ~Function() {}

    template<typename... Args>
    void operator()(Args... args) {
        if (this->handle == LUA_NOREF) {
            return;
        }
        this->evalFunc(0, args...);
    }

private:

    void evalFunc(int numArgs) {
        this->push();
        lua_insert(this->L, (numArgs+1)*-1); // insert after all the args
        if (lua_pcall(L, numArgs, 0, 0) != 0) {
            //TODO what this do
        }
    }

    template<typename T, typename... Args>
    void evalFunc(int numArgs, T t, Args... args) {
        pushValue(this->L, t);
        this->evalFunc(numArgs + 1, args...);
    }

};