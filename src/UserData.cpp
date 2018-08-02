#include "UserData.hpp"
#include "lua.hpp"

namespace mun {

// UserData::UserData(lua_State* L, const char* mtName, size_t size) :
//     mtName(mtName)
// {
//     lua_newuserdata(L, size);
//     this->handle = luaL_ref(L, LUA_REGISTRYINDEX);
//     lua_pop(L, -1);
// }

UserData::UserData(lua_State* L, int index) :
    Ref(L, index)
{
    
}

}