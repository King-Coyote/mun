#pragma once

#include <string>
#include "lua.hpp"
#include "Ref.hpp"
#include "StackOps.hpp"
#include "UserData.hpp"
#include "ClassBinding.hpp"
#include <iostream>

namespace lvm {
    class LuaVM;
}

/**
 * \brief provides an abstraction to the lua stack.
 * 
 * This class returns Ref objects using the [] operator.
 */
class lvm::LuaVM {
public:
    LuaVM();
    virtual ~LuaVM();

    /**
     * \brief opens the Lua libraries in prep for running scripts.
     */
    void openLibs();
    /**
     * \brief runs a lua script. Globals can be retreived from the vm using [] operator.
     */
    void runScript(std::string filename);
    /**
     * \brief run an arbitrary raw string literal in the vm
     * \param the string you want to run as a script - should be a raw string literal
     */
    void runString(std::string scriptString);
    /**
     * @brief Prints the stack in a nice redable form
     * @param msg the string to header the printout with
     */
    void showStack(const char* msg);

    /**
     * @brief creates a new global metatable in the state and associates it with functions
     * @param name the string name of the MT
     * @param funcs the array of functions to use of format int func(lua_State* L)
     */
    void createNewMetatable(const char* name, luaL_Reg* funcs = {});

    /**
     * @brief binds a class of given name to an empty metatable
     * the class binding is returned so you can operate on it inplace like .def(method)
     */
    template<typename T>
    ClassBinding<T> bindClass(const char* name, T* objPointer) {
        return ClassBinding<T>(name, this->L, objPointer);
    }

    /**
     * @brief same as bindClass but the class is bound as a global so that eg you could say
     * local button = Gui:newButton() for binding to gui.
     */
    template<typename T>
    ClassBinding<T> bindGlobalClass(const char* name, T* objPointer) {
        return ClassBinding<T>(name, this->L, objPointer, true);
    }

    /**
     * @brief creates a new userdata for the specififed class type and associate with the given metatable
     * @param mtName the string name of the metatable for the userdata
     * @return a pointer of the type specified
     */
    template<typename T>
    T** createUserdata(const char* mtName) {
        T** t = (T**)lua_newuserdata(this->L, sizeof(T*));
        lua_getglobal(this->L, mtName);
        lua_setmetatable(this->L, -2);
        return t;
    }

    /**
     * Creates a reference from a file. The file should return the object
     * you want to create. For example, the lua file:
     * local button = {
     *      width = 100,
     *      height = 50 
     * }
     * return button
     * will create a table from that object.
     */
    template<typename R>
    R refFromFile(const std::string& filename) {
        R r;
        if (luaL_dofile(this->L, filename.c_str()) != 0) {
            cout << lua_tostring(this->L, -1) << endl;
            return r;
        }
        getValue(this->L, r);
        lua_settop(this->L, 0);
        return r;
    }

    template<typename R>
    R refFromString(const std::string& script) {
        R r;
        if (luaL_dostring(this->L, script.c_str()) != 0) {
            cout << lua_tostring(this->L, -1) << endl;
            return r;
        }
        getValue(this->L, r);
        lua_settop(this->L, 0);
        return r;
    }

    template<typename R>
    R operator[](std::string key) {
        R r;
        lua_getglobal(this->L, key.c_str());
        getValue(this->L, r);
        lua_settop(this->L, 0);
        return r;
    }

    //DELETEME
    lua_State* getState() {return this->L;}

private:
    lua_State* L;

    // template<memFunc f>
    // int translateMethod(lua_State* L) {
        
    // }

};