#pragma once

#include <string>
#include "lua.hpp"

using namespace std;

namespace mun {
    class Table;
    class Ref;
    class Function;
    class UserData;

    void pushValue(lua_State* L, const int val);
    void pushValue(lua_State* L, const double val);
    void pushValue(lua_State* L, const char* val);
    void pushValue(lua_State* L, int (*val) (lua_State*));
    void pushValue(lua_State* L, const bool val);
    void pushValue(lua_State* L, const Ref& ref);
    void getValue(lua_State* L, mun::Table& val);
    void getValue(lua_State* L, mun::UserData& val);
    void getValue(lua_State* L, mun::Ref& val);
    void getValue(lua_State* L, mun::Function& val);
    void getValue(lua_State* L, int& val);
    void getValue(lua_State* L, double& val);
    void getValue(lua_State* L, const char*& val);
    void getValue(lua_State* L, const string& val);
    void getValue(lua_State* L, bool& val);
    void printStack(lua_State* L, const char* msg);
}