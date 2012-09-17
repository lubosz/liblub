#include "MeshBuffer.h"

MeshBuffer::MeshBuffer(string name, GLuint id, GLint size)
{
    this->id = id;
    this->name = name;
    this->size = size;
}
void MeshBuffer::init()
{
    glGenBuffers(1, &handle);
    /* Bind the first VBO as being the active buffer
     * and storing vertex attributes (coordinates) */
    glBindBuffer(GL_ARRAY_BUFFER, handle);

  /* Specify that our coordinate data is going
   * into attribute index bufferCount, and contains three floats per vertex */
  glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, 0, 0);

  /* Enable attribute index 0 as being used */
  glEnableVertexAttribArray(id);
    LogDebug << "Adding Vertex Buffer #" << id << name << handle;
}

void MeshBuffer::write(const vector<GLfloat> &content) {
  glBindBuffer(GL_ARRAY_BUFFER, handle);

  /* Copy the vertex data from mesh to our buffer */
  /* 12 * sizeof(GLfloat) is the size of the
   * mesh array, since it contains 12 GLfloat values */
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(content.size() * sizeof(GLfloat)),
            content.data(), GL_STATIC_DRAW);
}


void MeshBuffer::write_dynamic(const vector<GLfloat> &content) {
  glBindBuffer(GL_ARRAY_BUFFER, handle);

  /* Copy the vertex data from mesh to our buffer */
  /* 12 * sizeof(GLfloat) is the size of the
   * mesh array, since it contains 12 GLfloat values */
//  glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, 0, 0);
//  glEnableVertexAttribArray(id);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(content.size() * sizeof(GLfloat)),
            content.data(), GL_DYNAMIC_DRAW);
}
