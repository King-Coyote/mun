
#include <iostream>
#include "Ref.hpp"
#include "Table.hpp"
#include "Function.hpp"
#include "StackOps.hpp"

using namespace std;

namespace mun {

Ref::Ref() :
    L(nullptr),
    handle(LUA_NOREF)
{}

Ref::Ref(const Ref& other)
{
    *this = other;
}

Ref::Ref(lua_State* L, int index) :
    L(L)
{
    lua_pushvalue(L, index);
    this->handle = luaL_ref(L, LUA_REGISTRYINDEX);
}

Ref::~Ref() {
    luaL_unref(this->L, LUA_REGISTRYINDEX, this->handle);
}

void Ref::push() const {
    lua_rawgeti(this->L, LUA_REGISTRYINDEX, this->handle);
}

const Ref& Ref::operator=(const Ref& rhs) {
    L = rhs.L;
    this->handle = rhs.copyHandle();
    return *this;
}

Ref::operator bool() {
    return (this->handle != LUA_REFNIL);
}

int Ref::copyHandle() const {
    lua_rawgeti(this->L, LUA_REGISTRYINDEX, this->handle);
    return luaL_ref(this->L, LUA_REGISTRYINDEX);
}

int Ref::getHandle() {
    return this->handle;
}

// CAST PARTIAL SPECIALIZATIONS
template<>
int Ref::cast<int>() {
    this->push();
    if (!lua_isinteger(this->L, -1)) {
        return 0;
    }
    int ret = lua_tointeger(this->L, -1);
    lua_pop(this->L, -1);
    return ret;
}
template<>
string Ref::cast<string>() {
    this->push();
    if (!lua_isstring(this->L, -1)) {
        return "";
    }
    string ret = lua_tostring(this->L, -1);
    lua_pop(this->L, -1);
    return ret;
}
template<>
double Ref::cast<double>() {
    this->push();
    if (!lua_isnumber(this->L, -1)) {
        return 0.0;
    }
    double ret = lua_tonumber(this->L, -1);
    lua_pop(this->L, -1);
    return ret;
}

template<>
mun::Table Ref::cast<mun::Table>() {
    // push my data onto the stack, then create a new Table from that data e.g. copy this data
    this->push();
    if (!lua_istable(this->L, -1)) {
        // TODO what do here
    }
    mun::Table ret = mun::Table(this->L, -1);
    lua_pop(this->L, -1);
    return ret;
}

template<>
mun::Function Ref::cast<mun::Function>() {
    this->push();
    if (!lua_isfunction(this->L, -1)) {
        // TODO how to give invalid function
    }
    mun::Function ret = mun::Function(this->L, -1);
    lua_pop(this->L, -1);
    return ret;
}

}