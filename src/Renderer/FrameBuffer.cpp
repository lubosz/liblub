/*
 * Copyright © 2010 Lubosz Sarnecki
 * FrameBuffer.cpp
 *
 *  Created on: Aug 31, 2010
 */
#include <sstream>
#include <string>
#include "Renderer/FrameBuffer.h"
#include "Scene/SceneGraph.h"
#include "Scene/Camera.h"
#include "Mesh/Geometry.h"
#include "System/Logger.h"

using std::stringstream;

FrameBuffer::FrameBuffer(QSize& res) : res(res) {
  attachmentPoint = GL_COLOR_ATTACHMENT0;
    QList<string> attributes;
    attributes.push_back("uv");

    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    // create a renderbuffer object to store depth info
    // NOTE: A depth renderable image should be attached the FBO for depth test.
    // If we don't attach a depth renderable image to the FBO, then
    // the rendering output will be corrupted because of missing depth test.
    // If you also need stencil test for your rendering, then you must
    // attach additional image to the stencil attachement point, too.


    glGenRenderbuffers(1, &renderBufferHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, res.width(), res.height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, handle);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

//     GLuint stencilbuffer;                       // ID of Renderbuffer object
//     glGenRenderbuffers(1, &stencilbuffer);
//     glBindRenderbuffer(GL_RENDERBUFFER, stencilbuffer);
//     glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX1, res.width(), res.height());
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, handle);
//     glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glError;
}

void FrameBuffer::check() {
  // check FBO status
  bind();
  printFramebufferInfo();
  checkFramebufferStatus();
  unBind();
}

void FrameBuffer::attachTexture(Texture * texture) {
    bind();
    if (!texture->isDepth) {
        LogDebug << "Attaching color texture" << texture->name;
        // attach a texture to FBO color attachement point
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D,
                texture->getHandle(), 0);
        attachmentPoint++;
    } else {
        LogDebug << "Attaching depth texture" << texture->name;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D, texture->getHandle(), 0);
    }
}

void FrameBuffer::attachTextures(vector<Texture*> &textures) {
  if (textures.size() == 0)
    LogFatal << "No Textures";
  unsigned colorTextures = 0;
  foreach(Texture* texture, textures) {
    attachTexture(texture);
    if(!texture->isDepth)
      colorTextures++;
  }
  setDrawBuffers(colorTextures);
}


void FrameBuffer::disableColorBuffer() {
  bind();
    // disable color buffer if you don't attach any color buffer image,
    // for example, rendering depth buffer only to a texture.
    // Otherwise, glCheckFramebufferStatusEXT will not be complete.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void FrameBuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void FrameBuffer::setDrawBuffer(GLenum buffer) {
  // Set the render target
  bind();
  glDrawBuffer(buffer);
  glError;
}

void FrameBuffer::setDrawBuffers(vector<GLenum>& buffers) {
  bind();
  glDrawBuffers(buffers.size(), buffers.data());
}

void FrameBuffer::setDrawBuffers(unsigned count) {
  // Multiple render targets
  bind();

  vector<GLenum> buffers;
  for (unsigned i = 0; i < count; i++) {
    buffers.push_back(GL_COLOR_ATTACHMENT0_EXT+i);
  }
  glDrawBuffers(count, buffers.data());

  glError;
}

void FrameBuffer::unBind() {
    // back to normal window-system-provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::updateRenderView() {
    glViewport(0, 0, res.width(), res.height());
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &handle);
    glDeleteRenderbuffers(1, &renderBufferHandle);
}

void FrameBuffer::printFramebufferInfo() {
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);

    int objectType;
    int objectId;

    // print info of the colorbuffer attachable image
    for (int i = 0; i < colorBufferCount; ++i) {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + i,
                GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &objectType);
        if (objectType != GL_NONE) {
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0 + i,
                    GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &objectId);

            string formatName;

            LogDebug << "Color Attachment " << i << ": ";
            if (objectType == GL_TEXTURE)
              LogDebug << "GL_TEXTURE, "
                        << getTextureParameters(objectId);
            else if (objectType == GL_RENDERBUFFER)
              LogDebug << "GL_RENDERBUFFER, "
                        << getRenderbufferParameters(objectId);
        }
    }

    // print info of the depthbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &objectType);
    if (objectType != GL_NONE) {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                &objectId);

        LogDebug << "Depth Attachment: ";
        switch (objectType) {
        case GL_TEXTURE:
            LogDebug << "GL_TEXTURE, "
                    << getTextureParameters(objectId);
            break;
        case GL_RENDERBUFFER:
            LogDebug << "GL_RENDERBUFFER, "
                    << getRenderbufferParameters(objectId);
            break;
        }
    }

    // print info of the stencilbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
            GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
            &objectType);
    if (objectType != GL_NONE) {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                &objectId);

        LogDebug << "Stencil Attachment: ";
        switch (objectType) {
        case GL_TEXTURE:
            LogDebug << "GL_TEXTURE, "
                    << getTextureParameters(objectId);
            break;
        case GL_RENDERBUFFER:
            LogDebug << "GL_RENDERBUFFER, "
                    << getRenderbufferParameters(objectId);
            break;
        }
    }
}

string FrameBuffer::getTextureParameters(GLuint id) {
    if (glIsTexture(id) == GL_FALSE)
        return "Not texture object";

    int width, height, format;
    string formatName;
    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(
            GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
    glBindTexture(GL_TEXTURE_2D, 0);

    formatName = convertInternalFormatToString(format);

    stringstream ss;
    ss << width << "x" << height << ", " << formatName;
    return ss.str();
}

string FrameBuffer::getRenderbufferParameters(GLuint id) {
    if (glIsRenderbuffer(id) == GL_FALSE)
        return "Not Renderbuffer object";

    int width, height, format;
    string formatName;
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glGetRenderbufferParameteriv(
            GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(
            GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    glGetRenderbufferParameteriv(
            GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    formatName = convertInternalFormatToString(format);

    stringstream ss;
    ss << width << "x" << height << ", " << formatName;
    return ss.str();
}

string FrameBuffer::convertInternalFormatToString(GLenum format) {
    std::string formatName;

    switch (format) {
    case GL_STENCIL_INDEX:
        formatName = "GL_STENCIL_INDEX";
        break;
    case GL_DEPTH_COMPONENT:
        formatName = "GL_DEPTH_COMPONENT";
        break;
    case GL_ALPHA:
        formatName = "GL_ALPHA";
        break;
    case GL_RGB:
        formatName = "GL_RGB";
        break;
    case GL_RGBA:
        formatName = "GL_RGBA";
        break;
    case GL_R3_G3_B2:
        formatName = "GL_R3_G3_B2";
        break;
    case GL_RGB4:
        formatName = "GL_RGB4";
        break;
    case GL_RGB5:
        formatName = "GL_RGB4";
        break;
    case GL_RGB8:
        formatName = "GL_RGB8";
        break;
    case GL_RGB10:
        formatName = "GL_RGB10";
        break;
    case GL_RGB12:
        formatName = "GL_RGB12";
        break;
    case GL_RGB16:
        formatName = "GL_RGB16";
        break;
    case GL_RGBA2:
        formatName = "GL_RGBA2";
        break;
    case GL_RGBA4:
        formatName = "GL_RGBA4";
        break;
    case GL_RGB5_A1:
        formatName = "GL_RGB5_A1";
        break;
    case GL_RGBA8:
        formatName = "GL_RGBA8";
        break;
    case GL_RGB10_A2:
        formatName = "GL_RGB10_A2";
        break;
    case GL_RGBA12:
        formatName = "GL_RGBA12";
        break;
    case GL_RGBA16:
        formatName = "GL_RGBA16";
        break;
    case GL_DEPTH_COMPONENT32F:
      formatName = "GL_DEPTH_COMPONENT32F";
      break;
    case GL_RGBA32F:
      formatName = "GL_RGBA32F";
      break;
    default:
        formatName = "Unknown Format";
        LogWarning << "Unknown Texture Format" << ("0x"+QString::number(format, 16)).toStdString();
    }

    return formatName;
}

void FrameBuffer::checkFramebufferStatus() {
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
      LogDebug << "Framebuffer complete.";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      LogError << "Framebuffer incomplete: Attachment is NOT complete.";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      LogError << "Framebuffer incomplete: No image is attached to FBO.";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      LogError << "Framebuffer incomplete: Draw buffer.";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      LogError << "Framebuffer incomplete: Read buffer.";
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
      LogError << "Unsupported by FBO implementation.";
      break;
    default:
      LogError << "Unknow FBO error.";
  }
}
