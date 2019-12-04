
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "lua.hpp"
#include "State.hpp"
#include "Ref.hpp"
#include "Table.hpp"
#include "Function.hpp"
#include "StackOps.hpp"
#include "ClassBinding.hpp"

using namespace std;

class dummy {
public:
    dummy(int width, int height, string id) : width(width), height(height), id(id) {}

    int lua_onClick(lua_State* L) {
        this->funco = mun::Function(L, 3);
        return 0;
    }

    int lua_setHeight(lua_State* L) {
        this->height = lua_tointeger(L, 2);
        cout << "my height is now " << this->height << endl;
        return 0;
    }
    
    int lua_setWidth(lua_State* L) {
        this->width = lua_tointeger(L, 2);
        cout << "my width is now " << this->width << endl;
        return 0;
    }

    int width;
    int height;
    string id;
    mun::Function funco;
    mun::Ref luaRef;
};

class DummyContainer;

class DummyContainer {
public:
    DummyContainer() {
        this->lua.openLibs();
        this->lua.bindGlobalClass<DummyContainer>("Gui", this)
        .def<&DummyContainer::lua_newdummy>("newDummy");
    }

    // move constructor
    DummyContainer(DummyContainer&& dc) :
        DummyContainer()
    {
        for (auto& ptr : dc.dummies) {
            this->dummies.push_back(move(ptr));
        }
    }

    DummyContainer(const DummyContainer& copy) = delete;

    ~DummyContainer() {
        this->dummies.clear();
    }

    vector<unique_ptr<dummy>> dummies;
    mun::State lua;

    void handleClick() {
        for (auto& dum : this->dummies) {
            dum->funco();
        }
    }

    void runString(string script) {
        this->lua.runString(script);
    }

    int lua_print(lua_State* L) {
        cout << "fukk" << endl;
        return 0;
    }

    int lua_newdummy(lua_State* L) {
        // dummy** d = this->lua.createUserdata<dummy>("GuiObject");
        dummy* d = new dummy(0, 0, "abcdef");

        this->lua.bindClass<dummy>("GuiObject", d)
        .def<&dummy::lua_onClick>("onClick")
        .def<&dummy::lua_setHeight>("setHeight")
        .def<&dummy::lua_setWidth>("setWidth")
        .push();

        this->dummies.push_back(unique_ptr<dummy>(d));
        return 1;
    }

};

int main(int argc, char** argv) {

    mun::State s = mun::State();
    s.openLibs();

    s.runString(R"(
        local t = {
            id = "duhhh"
        }
        return t
    )");
    mun::Table t(s.getStatePointer(), -1);
    const char* kunt = t.get<const char*>("kunt", "shitcunt");
    cout << kunt << endl;


}