/*
 * Copyright © 2010 Lubosz Sarnecki
 * Geomerty.cpp
 *
 *  Created on: Mar 24, 2010
 */
#include <math.h>
#include <vector>
#include <random>
#include <functional>
#include "Procedural/Geometry.h"
#include "System/Logger.h"
#include "Renderer/OpenGL.h"

#include <QRectF>
#include <QMatrix4x4>

Mesh * Geometry::plane(const QList<string> & attributes, const QRectF &plane) {
    Mesh * mesh = new Mesh(attributes);

    mesh->buffers["position"] = {
        static_cast<float>(plane.left()), static_cast<float>(plane.top()), -1.0,
        static_cast<float>(plane.right()), static_cast<float>(plane.top()), -1.0,
        static_cast<float>(plane.right()), static_cast<float>(plane.bottom()), -1.0,
        static_cast<float>(plane.left()), static_cast<float>(plane.bottom()), -1.0
    };
    mesh->buffers["uv"] = {
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
    };
    mesh->buffers["normal"] = {
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0
    };
    mesh->indices = { 0, 1, 3, 3, 1, 2 };

    mesh->init();
    mesh->setDrawType(GL_TRIANGLES);
    glError;
    return mesh;
}

Mesh * Geometry::tetrahedron(const QList<string> & attributes) {
    Mesh * mesh = new Mesh(attributes);
    mesh->buffers["position"] = {
            1.0, 1.0, 1.0,
            -1.0, -1.0, 1.0,
            -1.0, 1.0, -1.0,
            1.0, -1.0, -1.0
    };

    mesh->buffers["color"] = {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 1.0, 1.0
    };

    mesh->buffers["uv"] = {
            1.0, 0.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 1.0
    };

    mesh->buffers["normal"] = {
            1.0, 1.0, 1.0,
            -1.0, -1.0, 1.0,
            -1.0, 1.0, -1.0,
            1.0, -1.0, -1.0 };

    mesh->indices = { 0, 1, 2, 3, 0, 1 };

    mesh->init();
    mesh->setDrawType(GL_TRIANGLE_STRIP);
    return mesh;
}

Mesh * Geometry::cube(const QList<string> & attributes) {
    Mesh * mesh = new Mesh(attributes);
    mesh->buffers["position"] = {
        1.0, -1.0, -1.0,
        1.0, -1.0, 1.0,
        -1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0,
        1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0
    };
    mesh->buffers["color"] = {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 1.0, 1.0,
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 1.0, 1.0
    };
    mesh->buffers["uv"] = {
        1.0, 0.0,
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0
    };
    mesh->indices = {
            4, 0, 3, 4, 3, 7,
            1, 5, 2, 5, 6, 2,
            2, 6, 7, 2, 7, 3,
            0, 4, 1, 4, 5, 1,
            4, 7, 5, 7, 6, 5,
            0, 1, 2, 0, 2, 3
    };
    mesh->init();
    mesh->setDrawType(GL_TRIANGLES);
    return mesh;
}

Mesh * Geometry::icosahedron(const QList<string> & attributes) {
  Mesh * mesh = new Mesh(attributes);
  mesh->indices = {
        2, 1, 0,
        3, 2, 0,
        4, 3, 0,
        5, 4, 0,
        1, 5, 0,

        11, 6,  7,
        11, 7,  8,
        11, 8,  9,
        11, 9,  10,
        11, 10, 6,

        1, 2, 6,
        2, 3, 7,
        3, 4, 8,
        4, 5, 9,
        5, 1, 10,

        2,  7, 6,
        3,  8, 7,
        4,  9, 8,
        5, 10, 9,
        1, 6, 10
  };

  mesh->buffers["position"] = {
         0.000f,  0.000f,  1.000f,
         0.894f,  0.000f,  0.447f,
         0.276f,  0.851f,  0.447f,
        -0.724f,  0.526f,  0.447f,
        -0.724f, -0.526f,  0.447f,
         0.276f, -0.851f,  0.447f,
         0.724f,  0.526f, -0.447f,
        -0.276f,  0.851f, -0.447f,
        -0.894f,  0.000f, -0.447f,
        -0.276f, -0.851f, -0.447f,
         0.724f, -0.526f, -0.447f,
         0.000f,  0.000f, -1.000f
  };

  mesh->init();
  return mesh;
}

Mesh * Geometry::stars(const QList<string> & attributes, const QVector3D & resolution, const float &density,
    const float &randomness, const float &colorIntensity) {

  Mesh * mesh = new Mesh(attributes);
  unsigned i = 0;

  //std::uniform_int_distribution<int> distribution(0, resolution.x());
  std::exponential_distribution<> distribution(1);

  std::random_device rd;
  std::mt19937 engine(rd());

  LogDebug << "Density: " << density
          << "Resolution: " << resolution.x() << resolution.y() << resolution.z()
          << "Randomness: " << randomness
          << "Color Intensity " << colorIntensity;

  for(float x = 0; x < resolution.x(); x++) {
    for(float y = 0; y < resolution.y(); y++) {
      for(float z = 0; z < resolution.z(); z++) {

        float vecX = distribution(engine) * resolution.x() / 10;
        float vecY = distribution(engine) * resolution.x() / 10;
        float vecZ = distribution(engine) * resolution.x() / 10;

        mesh->vertex("position", vecX, vecY, vecZ);
        mesh->vertex("position", vecX, -vecY, vecZ);
        mesh->vertex("position", vecX, vecY, -vecZ);
        mesh->vertex("position", vecX, -vecY, -vecZ);

        mesh->vertex("position", -vecX, vecY, vecZ);
        mesh->vertex("position", -vecX, -vecY, vecZ);
        mesh->vertex("position", -vecX, vecY, -vecZ);
        mesh->vertex("position", -vecX, -vecY, -vecZ);

        // Mirror exponential distribution on all axes
        for (unsigned octants = 0; octants < 8; octants++)
            mesh->vertex("color",
                1.0-(vecX * colorIntensity / 256.0),
                1.0-(vecY * colorIntensity / 256.0),
                1.0-(vecZ * colorIntensity/ 256.0));
        i++;
      }
    }
  }
  LogDebug << i << "stars generated";
  mesh->init();
  mesh->setDrawType(GL_POINTS);
  return mesh;
}

Mesh * Geometry::spiral(const QList<string> & attributes, int resolution) {
  Mesh * mesh = new Mesh(attributes);
  QVector3D point = QVector3D(1, 0, 0);
  QMatrix4x4 rotation = QMatrix4x4();
  rotation.rotate(0.5, 0.0, 1.0, 0.0);

  QMatrix4x4 rotation2 = QMatrix4x4();
  rotation2.rotate(0.5, 0.0, 0.0, 1.0);

  int i = 1;
#ifndef LIBLUB_WINDOWS
  srand(time(NULL));
#endif
  for (; i < resolution; i++) {
    QVector3D tempPoint = point.normalized();
    point += (tempPoint / 20.0);

    point = rotation * point;

    QVector3D newPoint = point;
    for (int j = 0; j < 100; j++) {
      newPoint = rotation2 * newPoint;

      QVector3D tempPoint2 = newPoint.normalized();
      newPoint += (tempPoint2 / 20.0);

      mesh->vertex("position",newPoint.x(), newPoint.y(), newPoint.z());
      mesh->vertex("color",
          static_cast<float> (static_cast<int> (newPoint.x()) % 255) / 256,
          static_cast<float> (static_cast<int> (newPoint.y()) % 255) / 256,
          static_cast<float> (static_cast<int> (newPoint.z()) % 255) / 256);
    }
  }

  LogInfo << i << "spiral points generated";

  mesh->init();
  mesh->setDrawType(GL_POINTS);
  return mesh;
}

//From gluSphere (MESA)
Mesh * Geometry::sphere(const QList<string> & attributes, GLdouble radius, GLint slices, GLint stacks) {

  Mesh * middleStrip = new Mesh(attributes);
  Mesh * bottomFan = new Mesh(attributes);
  Mesh * topFan = new Mesh(attributes);

  middleStrip->name = "Middle Strip";
  topFan->name = "topFan";
  bottomFan->name = "bottomFan";

  bool hasNormals = attributes.contains("normal");
  bool smoothNormals = true;
  bool drawPoints = false;
  bool drawLines = false;
  bool orientationOutside = true;
  bool useTextureCoords = attributes.contains("uv");
  GLenum drawStyle = GL_FILL;
#define CACHE_SIZE  240
  GLint i, j;
  GLfloat sinCache1a[CACHE_SIZE];
  GLfloat cosCache1a[CACHE_SIZE];
  GLfloat sinCache2a[CACHE_SIZE];
  GLfloat cosCache2a[CACHE_SIZE];
  GLfloat sinCache3a[CACHE_SIZE];
  GLfloat cosCache3a[CACHE_SIZE];
  GLfloat sinCache1b[CACHE_SIZE];
  GLfloat cosCache1b[CACHE_SIZE];
  GLfloat sinCache2b[CACHE_SIZE];
  GLfloat cosCache2b[CACHE_SIZE];
  GLfloat sinCache3b[CACHE_SIZE];
  GLfloat cosCache3b[CACHE_SIZE];
  GLfloat angle;
  GLfloat zLow, zHigh;
  GLfloat sintemp1 = 0.0, sintemp2 = 0.0, sintemp3 = 0.0, sintemp4 = 0.0;
//    GLfloat costemp1 = 0.0, costemp2 = 0.0;
  GLfloat costemp3 = 0.0, costemp4 = 0.0;
  GLboolean needCache2, needCache3;
  GLint start, finish;

  if (slices >= CACHE_SIZE)
    slices = CACHE_SIZE - 1;
  if (stacks >= CACHE_SIZE)
    stacks = CACHE_SIZE - 1;
//    if (slices < 2 || stacks < 1 || radius < 0.0) {
//      gluQuadricError(qobj, GL_INVALID_VALUE);
//      return;
//    }

  /* Cache is the vertex locations cache */
  /* Cache2 is the various normals at the vertices themselves */
  /* Cache3 is the various normals for the faces */
  needCache2 = needCache3 = GL_FALSE;

  if (smoothNormals) {
    needCache2 = GL_TRUE;
  } else {
    if (!drawPoints) {
      needCache3 = GL_TRUE;
    }
    if (drawLines) {
      needCache2 = GL_TRUE;
    }
  }

  for (i = 0; i < slices; i++) {
    angle = 2 * M_PI * i / slices;
    sinCache1a[i] = sin(angle);
    cosCache1a[i] = cos(angle);
    if (needCache2) {
      sinCache2a[i] = sinCache1a[i];
      cosCache2a[i] = cosCache1a[i];
    }
  }

  for (j = 0; j <= stacks; j++) {
    angle = M_PI * j / stacks;
    if (needCache2) {
      if (orientationOutside) {
        sinCache2b[j] = sin(angle);
        cosCache2b[j] = cos(angle);
      } else {
        sinCache2b[j] = -sin(angle);
        cosCache2b[j] = -cos(angle);
      }
    }
    sinCache1b[j] = radius * sin(angle);
    cosCache1b[j] = radius * cos(angle);
  }
  /* Make sure it comes to a point */
  sinCache1b[0] = 0;
  sinCache1b[stacks] = 0;

  if (needCache3) {
    for (i = 0; i < slices; i++) {
      angle = 2 * M_PI * (i - 0.5) / slices;
      sinCache3a[i] = sin(angle);
      cosCache3a[i] = cos(angle);
    }
    for (j = 0; j <= stacks; j++) {
      angle = M_PI * (j - 0.5) / stacks;
      if (orientationOutside) {
        sinCache3b[j] = sin(angle);
        cosCache3b[j] = cos(angle);
      } else {
        sinCache3b[j] = -sin(angle);
        cosCache3b[j] = -cos(angle);
      }
    }
  }

  sinCache1a[slices] = sinCache1a[0];
  cosCache1a[slices] = cosCache1a[0];
  if (needCache2) {
    sinCache2a[slices] = sinCache2a[0];
    cosCache2a[slices] = cosCache2a[0];
  }
  if (needCache3) {
    sinCache3a[slices] = sinCache3a[0];
    cosCache3a[slices] = cosCache3a[0];
  }

  switch (drawStyle) {
  case GL_FILL:
    /* Do ends of sphere as TRIANGLE_FAN's (if not texturing)
     ** We don't do it when texturing because we need to respecify the
     ** texture coordinates of the apex for every adjacent vertex (because
     ** it isn't a constant for that point)
     */
    if (!useTextureCoords) {
      start = 1;
      finish = stacks - 1;

      /* Low end first (j == 0 iteration) */
      sintemp2 = sinCache1b[1];
      zHigh = cosCache1b[1];

      if(smoothNormals){
        sintemp3 = sinCache2b[1];
        costemp3 = cosCache2b[1];
        topFan->vertex("normal",sinCache2a[0] * sinCache2b[0], cosCache2a[0]
            * sinCache2b[0], cosCache2b[0]);
      }else{
        sintemp3 = sinCache3b[1];
        costemp3 = cosCache3b[1];
      }

      //glBegin(GL_TRIANGLE_FAN);
      topFan->vertex("position",0.0, 0.0, radius);
      if (orientationOutside) {
        for (i = slices; i >= 0; i--) {
          if (smoothNormals) {
            topFan->vertex("normal",sinCache2a[i] * sintemp3,
                cosCache2a[i] * sintemp3, costemp3);
          } else {
            if (i != slices) {
              topFan->vertex("normal",sinCache3a[i + 1] * sintemp3,
                  cosCache3a[i + 1] * sintemp3, costemp3);
            }
          }
          topFan->vertex("position",sintemp2 * sinCache1a[i],
              sintemp2 * cosCache1a[i], zHigh);
        }
      } else {
        for (i = 0; i <= slices; i++) {
          if (smoothNormals) {
            topFan->vertex("normal",sinCache2a[i] * sintemp3,
                cosCache2a[i] * sintemp3, costemp3);
          } else {
            topFan->vertex("normal",sinCache3a[i] * sintemp3,
                cosCache3a[i] * sintemp3, costemp3);
          }
          topFan->vertex("position",sintemp2 * sinCache1a[i],
              sintemp2 * cosCache1a[i], zHigh);
        }
      }

      /* High end next (j == stacks-1 iteration) */
      sintemp2 = sinCache1b[stacks - 1];
      zHigh = cosCache1b[stacks - 1];

      if (smoothNormals) {
        sintemp3 = sinCache2b[stacks - 1];
        costemp3 = cosCache2b[stacks - 1];
        bottomFan->vertex("normal",sinCache2a[stacks] * sinCache2b[stacks],
            cosCache2a[stacks] * sinCache2b[stacks], cosCache2b[stacks]);
      } else {
        sintemp3 = sinCache3b[stacks];
        costemp3 = cosCache3b[stacks];
      }

      bottomFan->vertex("position",0.0, 0.0, -radius);
      if (orientationOutside) {
        for (i = 0; i <= slices; i++) {
          if (smoothNormals) {
            bottomFan->vertex("normal",sinCache2a[i] * sintemp3,
                cosCache2a[i] * sintemp3, costemp3);
          } else {
            bottomFan->vertex("normal",sinCache3a[i] * sintemp3,
                cosCache3a[i] * sintemp3, costemp3);
          }
          bottomFan->vertex("position",sintemp2 * sinCache1a[i],
              sintemp2 * cosCache1a[i], zHigh);
        }
      } else {
        for (i = slices; i >= 0; i--) {
          if (smoothNormals) {
            bottomFan->vertex("normal",sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3,
                costemp3);
          } else {
            if (i != slices) {
              bottomFan->vertex("normal",sinCache3a[i + 1] * sintemp3,
                  cosCache3a[i + 1] * sintemp3, costemp3);
            }
          }
          bottomFan->vertex("position",sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
        }
      }
      //glEnd();
    } else {
      start = 0;
      finish = stacks;
    }
    for (j = start; j < finish; j++) {
      zLow = cosCache1b[j];
      zHigh = cosCache1b[j + 1];
      sintemp1 = sinCache1b[j];
      sintemp2 = sinCache1b[j + 1];


      if (smoothNormals) {
        if (orientationOutside) {
          sintemp3 = sinCache2b[j + 1];
          costemp3 = cosCache2b[j + 1];
          sintemp4 = sinCache2b[j];
          costemp4 = cosCache2b[j];
        } else {
          sintemp3 = sinCache2b[j];
          costemp3 = cosCache2b[j];
          sintemp4 = sinCache2b[j + 1];
          costemp4 = cosCache2b[j + 1];
        }
      } else {
        sintemp4 = sinCache3b[j + 1];
        costemp4 = cosCache3b[j + 1];
      }

      //glBegin(GL_QUAD_STRIP);
      for (i = 0; i <= slices; i++) {
          if (smoothNormals)
            middleStrip->vertex("normal",sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3,
                costemp3);
        if (orientationOutside) {
          if (useTextureCoords) {
            middleStrip->vertex("uv",1 - static_cast<float> (i) / slices, 1 - static_cast<float> (j + 1) / stacks);
          }
          middleStrip->vertex("position",sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i],
              zHigh);
        } else {
//            if (useTextureCoords) {
//              glTexCoord2f(1 - (float) i / slices, 1 - (float) j / stacks);
//            }
//            vertex("position",sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], zLow);
        }
        if (hasNormals && smoothNormals) {
          middleStrip->vertex("normal",sinCache2a[i] * sintemp4, cosCache2a[i] * sintemp4,
              costemp4);
//            middleStrip->vertex("normal",sinCache3a[i] * sintemp4, cosCache3a[i] * sintemp4,
//                costemp4);
        }
        if (orientationOutside) {
          if (useTextureCoords) {
            middleStrip->vertex("uv",1 - static_cast<float>(i) / slices, 1 - static_cast<float>(j) / stacks);
          }
          middleStrip->vertex("position",sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], zLow);
        } else {
//            if (useTextureCoords) {
//              glTexCoord2f(1 - (float) i / slices, 1 - (float) (j + 1) / stacks);
//            }
//            vertex("position",sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i],
//                zHigh);
        }
      }
      //glEnd();
    }
    break;
//    case GL_POINT:
//      //glBegin(GL_POINTS);
//      for (j = 0; j <= stacks; j++) {
//        sintemp1 = sinCache1b[j];
//        costemp1 = cosCache1b[j];
//        if (hasNormals && smoothNormals) {
//          sintemp2 = sinCache2b[j];
//          costemp2 = cosCache2b[j];
//        }
//
//        for (i = 0; i < slices; i++) {
//          if (hasNormals && smoothNormals) {
//            vertex("normal",sinCache2a[i] * sintemp2, cosCache2a[i] * sintemp2,
//                costemp2);
//          }
//
//          zLow = j * radius / stacks;
//
//          if (useTextureCoords) {
//            glTexCoord2f(1 - (float) i / slices, 1 - (float) j / stacks);
//          }
//          vertex("position",sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i],
//              costemp1);
//        }
//      }
//      //glEnd();
//      break;
//    case GL_LINE:
//      for (j = 1; j < stacks; j++) {
//        sintemp1 = sinCache1b[j];
//        costemp1 = cosCache1b[j];
//        if (hasNormals && smoothNormals) {
//          sintemp2 = sinCache2b[j];
//          costemp2 = cosCache2b[j];
//        }
//
//        //glBegin(GL_LINE_STRIP);
//        for (i = 0; i <= slices; i++) {
//          if (hasNormals) {
//            if (smoothNormals) {
//            vertex("normal",sinCache2a[i] * sintemp2, cosCache2a[i] * sintemp2,
//                costemp2);
//            } else {
//            vertex("normal",sinCache3a[i] * sintemp2, cosCache3a[i] * sintemp2,
//                costemp2);
//            }
//          }
//          if (useTextureCoords) {
//            glTexCoord2f(1 - (float) i / slices, 1 - (float) j / stacks);
//          }
//          vertex("position",sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i],
//              costemp1);
//        }
//        //glEnd();
//      }
//      for (i = 0; i < slices; i++) {
//        sintemp1 = sinCache1a[i];
//        costemp1 = cosCache1a[i];
//        if (hasNormals && smoothNormals) {
//          sintemp2 = sinCache2a[i];
//          costemp2 = cosCache2a[i];
//        }
//
//        //glBegin(GL_LINE_STRIP);
//        for (j = 0; j <= stacks; j++) {
//          if (hasNormals) {
//            if (smoothNormals) {
//            vertex("normal",sintemp2 * sinCache2b[j], costemp2 * sinCache2b[j],
//                cosCache2b[j]);
//            } else {
//            vertex("normal",sintemp2 * sinCache3b[j], costemp2 * sinCache3b[j],
//                cosCache3b[j]);
//            }
//          }
//
//          if (useTextureCoords) {
//            glTexCoord2f(1 - (float) i / slices, 1 - (float) j / stacks);
//          }
//          vertex("position",sintemp1 * sinCache1b[j], costemp1 * sinCache1b[j],
//              cosCache1b[j]);
//        }
//        //glEnd();
//      }
//      break;
  default:
    break;
  }

  middleStrip->init();
  middleStrip->setDrawType(GL_TRIANGLE_STRIP);

  if(!useTextureCoords) {
    Mesh * sphere = new Mesh(attributes);
    topFan->init();
    topFan->setDrawType(GL_TRIANGLE_FAN);

    bottomFan->init();
    bottomFan->setDrawType(GL_TRIANGLE_FAN);

    sphere->addSubMesh(middleStrip);
    sphere->addSubMesh(topFan);
    sphere->addSubMesh(bottomFan);

    return sphere;
  } else {
    return middleStrip;
  }

}
