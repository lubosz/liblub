#ifndef MESHBUFFER_H
#define MESHBUFFER_H

#include <vector>
#include <string>
#include "Renderer/OpenGL.h"

using std::vector;
using std::string;

class MeshBuffer
{
public:

    GLuint handle;
    GLuint id;
    string name;
    GLuint size;

    MeshBuffer(string name, GLuint id, GLuint size);

    void init();
    void write(const vector<GLfloat> &content);
    void write_dynamic(const vector<GLfloat> &content);
};

#endif // MESHBUFFER_H
