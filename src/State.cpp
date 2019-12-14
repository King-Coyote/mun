#include <iostream>
#include <string>
#include "State.hpp"
#include "StackOps.hpp"

using namespace std;

namespace mun {

State::State() {
    this->L = luaL_newstate();
}

State::~State() {
    lua_close(this->L);
}

void State::openLibs() {
    luaL_openlibs(this->L);
}

void State::runScript(
    const string& filename,
    const function<void(const string&)>& errorCallback
) {
    if (luaL_dofile(this->L, filename.c_str()) != 0) {
        auto s = string(lua_tostring(this->L, -1));
        lua_pop(this->L, 1);
        if (errorCallback) {
            errorCallback(s);
        } else {
            throw std::runtime_error(s);
        }
    }
}

void State::runString(
    const string& scriptString,
    const function<void(const string&)>& errorCallback
) {
    if (luaL_dostring(this->L, scriptString.c_str()) != 0) {
        auto s = string(lua_tostring(this->L, -1));
        lua_pop(this->L, 1);
        if (errorCallback) {
            errorCallback(string(lua_tostring(this->L, -1)));
        } else {
            throw std::runtime_error(s);
        }
    }
}

void State::showStack(const char* msg) {
    printStack(this->L, msg);
}

void State::appendPackagePath(const string& path) {
    lua_getglobal( L, "package" );
    lua_getfield( L, -1, "path" ); // get field "path" from table at top of stack (-1)
    string curPath = string(lua_tostring( L, -1 )); // grab path string from top of stack
    curPath += ";";
    curPath += string(path);
    curPath += "/?.lua";
    lua_pop( L, 1 ); // get rid of the string on the stack
    lua_pushstring( L, curPath.c_str()); // push the new one
    lua_setfield( L, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop( L, 1 ); // get rid of package table from top of stack
}

void State::createNewMetatable(const char* name, luaL_Reg* funcs) {
    luaL_newmetatable(this->L, name);
    lua_pushvalue(this->L, -1);
    lua_setfield(this->L, -2, "__index");
    luaL_setfuncs(this->L, funcs, 0);
    lua_setglobal(this->L, name);
    lua_settop(this->L, 0);
}

}