#include "NamedObject.h"

NamedObject::NamedObject(string name) : name(name)
{
}

string NamedObject::getName() {
    return name;
}

void NamedObject::setName(string name) {
    this->name = name;
}
