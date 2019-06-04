
#include "Table.hpp"

using namespace std;

namespace mun {

unsigned Table::getLength() {
    this->push();
    size_t length = lua_rawlen(this->L, -1);
    lua_pop(this->L, 1);
    return length;
}

vector<string> Table::keys() {
    vector<string> keys = vector<string>();
    this->push();
    lua_pushnil(this->L);
    while(lua_next(this->L, -2)) {
        if (lua_isstring(L, -2)) {
            string key = lua_tostring(L,-2);
            keys.push_back(key);
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return keys;
}

vector<int> Table::indices() {
    vector<int> indices = vector<int>();
    this->push();
    lua_pushnil(this->L);
    int count = 1;
    while(lua_next(this->L, -2)) {
        indices.push_back(count++);
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return indices;
}

}