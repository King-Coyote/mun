#pragma once

#include <string>
#include "lua.hpp"

namespace mun {
    class Ref;
}

class mun::Ref {
public:
    Ref();
    Ref(const Ref& other);
    Ref(lua_State* L, int index = -1);
    virtual ~Ref();

    const Ref& operator=(const Ref& rhs);
    operator bool();

    int copyHandle() const;
    template<typename T>
    T cast();

    void push() const;

    int getHandle();

protected:

    lua_State* L;

    int handle;

private:

};