
#include <string>
#include "lua.hpp"
#include "StackOps.hpp"
#include "Ref.hpp"
#include "Table.hpp"
#include "Function.hpp"
#include "UserData.hpp"

using namespace std;

namespace mun {

void pushValue(lua_State* L, const int val) {
    lua_pushinteger(L, val);
}

void pushValue(lua_State* L, const double val) {
    lua_pushnumber(L, val);
}

void pushValue(lua_State* L, const char* val) {
    lua_pushstring(L, val);
}

void pushValue(lua_State* L, int (*val) (lua_State*)) {
    lua_pushcfunction(L, val);
}

void pushValue(lua_State* L, const bool val) {
    lua_pushboolean(L, val);
}

void pushValue(lua_State*, const Ref& val) {
    val.push();
}

void getValue(lua_State* L, mun::UserData& val) {
    val = mun::UserData(L, -1);
}

void getValue(lua_State* L, mun::Table& val) {
    val = mun::Table(L, -1);
}

void getValue(lua_State* L, mun::Ref& val) {
    val = mun::Ref(L, -1);
}

void getValue(lua_State* L, mun::Function& val) {
    val = mun::Function(L, -1);
}

void getValue(lua_State* L, int& val) {
    val = lua_tointeger(L, -1);
}

void getValue(lua_State* L, double& val) {
    val = lua_tonumber(L, -1);
}

void getValue(lua_State* L, const char*& val) {
    val = lua_tostring(L, -1);
}

void getValue(lua_State* L, string& val) {
    val = lua_tostring(L, -1);
}

void getValue(lua_State* L, bool& val) {
    val = lua_toboolean(L, -1);
}

void printStack(lua_State* L, const char* msg) {
  int i;
  int top=lua_gettop(L);
  printf("dumpstack -- %s\n",msg);
  for (i=1; i<=top; i++) {
    printf("%d\t%s\t",i,luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
  printf("dumpstack -- END\n");
}

}