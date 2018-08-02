
#include "Table.hpp"

using namespace std;

namespace mun {

unsigned Table::getLength() {
    this->push();
    size_t length = lua_rawlen(this->L, -1);
    lua_pop(this->L, 1);
    return length;
}

}