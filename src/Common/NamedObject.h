#ifndef NAMEDOBJECT_H
#define NAMEDOBJECT_H

#include <string>

using std::string;

class NamedObject {
 public:
    NamedObject(string name);

    string getName();
    void setName(string name);

 private:
    string name;
};

#endif // NAMEDOBJECT_H
