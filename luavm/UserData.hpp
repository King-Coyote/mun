#pragma once

#include "Ref.hpp"

namespace mun {

class UserData : public Ref {
public:
    // UserData(lua_State* L, const char* mtName, size_t size);
    UserData(lua_State* L, int index = -1);

private:
    // const char* mtName;
};

}