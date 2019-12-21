#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "lua.hpp"
#include "Ref.hpp"
#include "StackOps.hpp"

namespace mun {
    class Function;
}

typedef std::function<void(const std::string&)> ErrorCallback;

class mun::Function : public Ref {
public:
    Function() : Ref() {}
    Function(lua_State* L, int index = -1) : Ref(L, index) {}
    ~Function() {}

    template<typename... Args>
    void operator()(
        ErrorCallback& callback,
        Args... args
    ) {
        if (this->handle == LUA_NOREF) {
            return;
        }
        this->push();
        this->pushArg(args...);
        int numArgs = sizeof...(args);
        if (lua_pcall(L, numArgs, 0, 0) != 0) {
            auto msg = string(lua_tostring(this->L, -1));
            lua_pop(L, numArgs + 1);
            if (callback != nullptr) {
                callback(msg);
                return;
            } else {
                throw new std::runtime_error(msg);
            }
        }
    }

    template<typename R, typename... Args>
    R call(
        ErrorCallback& callback,
        Args... args
    ) {
        R r;
        if (this->handle == LUA_NOREF) {
            return r;
        }
        this->push();
        this->pushArg(args...);
        int numArgs = sizeof...(args);
        if (lua_pcall(L, numArgs, 1, 0) != 0) {
            auto msg = string(lua_tostring(this->L, -1));
            lua_pop(L, numArgs + 1);
            if (callback != nullptr) {
                callback(msg);
                return r;
            } else {
                throw new std::runtime_error(msg);
            }
        }
        getValue(L, r);
        return r;
    }

private:

    template<typename T, typename... Args>
    void pushArg(T&& a, Args&&... args) {
        pushValue(this->L, a);
        pushArg(args...);
    }

    void pushArg(){}
};