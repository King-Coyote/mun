#include <iostream>
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

void State::runScript(string filename) {
    if (luaL_dofile(this->L, filename.c_str()) != 0) {
        throw std::runtime_error("Script not valid!");
    }
}

void State::runString(string scriptString) {
    if (luaL_dostring(this->L, scriptString.c_str()) != 0) {
        throw std::runtime_error(lua_tostring(this->L, -1));
    }
}

void State::showStack(const char* msg) {
    printStack(this->L, msg);
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