#pragma once

#include "Ref.hpp"
#include "StackOps.hpp"
#include "Function.hpp"
#include <vector>

namespace mun {
    class Table;
}

class mun::Table : public Ref {
public:
    Table() : Ref() {}
    Table(lua_State* L, int index = -1) : Ref(L, index) {}
    ~Table() {}

    /**
     * \brief Allows for the creation of an empty table if given a state pointer.
     * Use it like: mun::Table t = mun::Table().createEmpty(state.getStatePointer());
     */
    mun::Table& createEmpty(lua_State* L);

    /**
     * \brief Get the number of values in the table
     * \return The length of the table eg #table in lua
     */
    unsigned getLength();

    /**
     * \brief Returns a iterable vector of string keys in the lua table
     */
    std::vector<std::string> keys();

    /**
     * \brief returns a vector of indices of the table
     */
    std::vector<int> indices();

    /**
     * \brief sets a key in the table to value
     */
    template<typename T>
    bool set(const char* key, const T& value) {
        this->push();
        if (!lua_istable(this->L, -1)) {
            lua_pop(this->L, 1);
            return false;
        }
        pushValue(L, value);
        lua_setfield(L, -2, key);
        lua_pop(L, 1);
        return true;
    }

    /**
     * \brief returns true if the table contains the key
     */
    template<typename T>
    bool contains(T& key) const {
        this->push();
        if (!lua_istable(this->L, -1)) {
            lua_pop(this->L, 1);
            return false;
        }
        pushValue(this->L, key);
        lua_gettable(this->L, -2);
        bool isInTable = false;
        getValue(this->L, isInTable);
        lua_pop(this->L, 2);
        return isInTable;
    }

    template<typename R, typename T>
    R get(T key) const {
        R r;
        this->push();
        if (!lua_istable(this->L, -1)) {
            lua_pop(this->L, 1);
            return r;
        }
        pushValue(this->L, key);
        lua_gettable(this->L, -2);
        getValue(this->L, r);
        lua_pop(this->L, 2);
        return r;
    }

    template<typename R, typename T>
    R get(T key, const R&& valueIfNil) const {
        R r;
        this->push();
        if (!lua_istable(this->L, -1)) {
            lua_pop(this->L, 1);
            return valueIfNil;
        }
        pushValue(this->L, key);
        lua_gettable(this->L, -2);
        if (lua_isnil(this->L, -1)) {
            lua_pop(this->L, 2);
            return valueIfNil;
        }
        getValue(this->L, r);
        lua_pop(this->L, 2);
        return r;
    }

    template<typename R, typename T>
    R get(T key, const R& valueIfNil) const {
        R r;
        this->push();
        if (!lua_istable(this->L, -1)) {
            lua_pop(this->L, 1);
            r = R(valueIfNil);
            return r;
        }
        pushValue(this->L, key);
        lua_gettable(this->L, -2);
        if (lua_isnil(this->L, -1)) {
            lua_pop(this->L, 2);
            r = R(valueIfNil);
            return r;
        }
        getValue(this->L, r);
        lua_pop(this->L, 2);
        return r;
    }

};