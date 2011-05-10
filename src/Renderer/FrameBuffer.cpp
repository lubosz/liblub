/*
 * Copyright © 2010 Lubosz Sarnecki
 * FrameBuffer.cpp
 *
 *  Created on: Aug 31, 2010
 */
#include <sstream>
#include <string>
#include "Renderer/FrameBuffer.h"
#include "Material/Materials.h"
#include "Scene/SceneGraph.h"
#include "Scene/Camera.h"
#include "Mesh/Geometry.h"
#include "System/Logger.h"

using std::stringstream;

FrameBuffer::FrameBuffer(GLuint width, GLuint height) {
    glError;
    // Gen texture for fbo
    // create a texture object
    this->width = width;
    this->height = height;

    QList<string> attributes;
    attributes.push_back("uv");

    renderPlane = Geometry::plane(attributes, QRectF(-1,-1,2,2));

    glError;

    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    // create a renderbuffer object to store depth info
    // NOTE: A depth renderable image should be attached the FBO for depth test.
    // If we don't attach a depth renderable image to the FBO, then
    // the rendering output will be corrupted because of missing depth test.
    // If you also need stencil test for your rendering, then you must
    // attach additional image to the stencil attachement point, too.


    glGenRenderbuffers(1, &rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboId);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

//     GLuint stencilbuffer;                       // ID of Renderbuffer object
//     glGenRenderbuffers(1, &stencilbuffer);
//     glBindRenderbuffer(GL_RENDERBUFFER, stencilbuffer);
//     glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX1, width, height);
//     glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glError;
}

void FrameBuffer::checkAndFinish() {
    // check FBO status
    printFramebufferInfo();
    LogDebug << checkFramebufferStatus();
    unBind();
}

void FrameBuffer::attachTexture(GLenum attachmentPoint, Texture * texture) {
    // attach a texture to FBO color attachement point
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D,
            texture->getHandle(), 0);
}

void FrameBuffer::disableColorBuffer() {
    // disable color buffer if you don't attach any color buffer image,
    // for example, rendering depth buffer only to a texture.
    // Otherwise, glCheckFramebufferStatusEXT will not be complete.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void FrameBuffer::bind() {
    // set the rendering destination to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    // Set the render target
    // glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glError;
}

void FrameBuffer::bindMulti() {
  // set the rendering destination to FBO
  glBindFramebuffer(GL_FRAMEBUFFER, fboId);

  // Multiple render targets
//  GLenum buffers[] = {
//      GL_COLOR_ATTACHMENT0_EXT,
//      GL_COLOR_ATTACHMENT1_EXT,
//      GL_COLOR_ATTACHMENT2_EXT,
//      GL_COLOR_ATTACHMENT3_EXT,
//      GL_COLOR_ATTACHMENT4_EXT,
//      GL_COLOR_ATTACHMENT5_EXT
//  };
//  glDrawBuffers(6, buffers);

  glError;
}

void FrameBuffer::unBind() {
    // back to normal window-system-provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::updateRenderView() {
    glViewport(0, 0, width, height);
}

void FrameBuffer::draw(Material * material) {
    material->getShaderProgram()->use();
    material->getShaderProgram()->setUniform("MVPMatrix",QMatrix4x4());
    material->activate();
    renderPlane->draw();
    glError;
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &fboId);
    glDeleteRenderbuffers(1, &rboId);
}

void FrameBuffer::printFramebufferInfo() {
    // print max # of colorbuffers supported by FBO
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
    LogDebug << "Max Number of Color Buffer Attachment Points: "
            << colorBufferCount;

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
    default:
        formatName = "Unknown Format";
    }

    return formatName;
}

string FrameBuffer::checkFramebufferStatus() {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
        return "Framebuffer complete.";

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        return "[ERROR] Framebuffer incomplete: Attachment is NOT complete.";

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        return "[ERROR] Framebuffer incomplete: No image is attached to FBO.";

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        return "[ERROR] Framebuffer incomplete: Draw buffer.";

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        return "[ERROR] Framebuffer incomplete: Read buffer.";

    case GL_FRAMEBUFFER_UNSUPPORTED:
        return "[ERROR] Unsupported by FBO implementation.";

    default:
        return "[ERROR] Unknow error.";
    }
}
