#pragma once

#include <string>
#include <sstream>
#include "lua.hpp"
#include "StackOps.hpp"
#include "Ref.hpp"

namespace mun {

#define GLOBAL_UDATA_STR "CI_classUdata"

template <typename T>
class ClassBinding {
public:
    ClassBinding(const char*, lua_State* L, T* objToBind, bool setAsGlobal = false);
    ~ClassBinding() {}

    template<int (T::*transFunc)(lua_State* L)>
    ClassBinding<T>& def(const char* methodName);

    template<typename R>
    ClassBinding<T>& setValue(const char* name, const R& value);

    // push the ref to the stack, do this if not binding a global class after defs
    ClassBinding<T>& push();
    // Get the ref object; useful for eg passing self to functions
    mun::Ref getRef();

private:
    /**
     * @brief changes a method into a pointer that Lua can use through the use of what I like to call
     * "computer thigns"
     * when this function is called, classbinding no longer exists
     * this means that 'touserdata' requires that the Lua object calling the function be the class that was registered for it
     */
    template<int (T::*transFunc)(lua_State* L)>
    static int transmuteMethod(lua_State* L) {
        // assuming the var is on top of the stack, check if it's a udata. If so, use as normal. If not, it
        // could be a table, so try to treat it as a global class pointer. If not, fkn get rekt
        T* ptr;
        if (lua_isuserdata(L, 1)) {
            ptr = *(T**)lua_touserdata(L, 1);
        } else if (lua_istable(L, 1)) {
            lua_getfield(L, 1, GLOBAL_UDATA_STR);
            //lua_remove(L, 1);
            ptr = *(T**)lua_touserdata(L, -1);
        } else {
            printStack(L, "Literally no idea what this is, have a look");
        }
        return ((*ptr).*transFunc)(L);
    }

    std::string name;
    Ref ref;
    lua_State* L;
};

template<typename T>
ClassBinding<T>::ClassBinding(const char* name, lua_State* L, T* objToBind, bool setAsGlobal) :
    name(name),
    L(L)
{
    lua_getglobal(L, name);
    if (lua_isnil(L, -1)) {
        // metatable does not exist, mek it
        lua_pop(L, 1);
        luaL_newmetatable(L, name);
        lua_pushvalue(this->L, -1);
        lua_setfield(this->L, -2, "__index");
        lua_pushvalue(L, -1);
        lua_setglobal(L, name);
    }
    T** ptr = (T**)lua_newuserdata(L, sizeof(T*));
    lua_pushvalue(L, -2);
    lua_setmetatable(L, -2);
    if (setAsGlobal) {
        lua_setfield(this->L, -2, GLOBAL_UDATA_STR);
        lua_pop(L, 1);
    } else {
        this->ref = Ref(L, -1);
        lua_pop(L, 2);
    }
    *ptr = objToBind;
}

template<typename T>
template<int (T::*transFunc)(lua_State* L)>
ClassBinding<T>& ClassBinding<T>::def(const char* methodName) {
    //lua_rawgeti(this->L, LUA_REGISTRYINDEX, this->handle);
    luaL_Reg transmutedFunc[] = {
        methodName, &ClassBinding::transmuteMethod<transFunc>,
        NULL, NULL
    };
    lua_getglobal(this->L, this->name.c_str());
    luaL_setfuncs(this->L, transmutedFunc, 0);
    lua_pop(this->L, 1);
    return *this;
}

template<typename T>
template<typename R>
ClassBinding<T>&  ClassBinding<T>::setValue(const char* name, const R& value) {
    lua_getglobal(this->L, this->name.c_str());
    pushValue(this->L, value);
    lua_setfield(this->L, -2, name);
    lua_pop(this->L, 1);
    return *this;
}

template<typename T>
ClassBinding<T>& ClassBinding<T>::push() {
    this->ref.push();
    return *this;
}

template<typename T>
mun::Ref ClassBinding<T>::getRef() {
    return this->ref;
}

}