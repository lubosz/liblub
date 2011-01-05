/*
 * Copyright © 2010 Lubosz Sarnecki
 * Geomerty.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#include "Geometry.h"
#include "Qt3D.h"

Mesh * Geometry::makePlane() {
    vector<GLfloat> vertices = {
            -1.0, -1.0, -1.0,
            1.0, -1.0, -1.0,
            1.0, 1.0, -1.0,
            -1.0, 1.0, -1.0 };

    vector<GLfloat> uvCoords = {
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
    };

    vector<GLfloat> normals = {
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0
    };

    vector<GLuint> indicies = { 0, 1, 3, 3, 1, 2 };

    Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices, 3, "in_Vertex");
    mesh->addBuffer(normals, 3, "in_Normal");
    mesh->addBuffer(normals, 3, "in_Tangent");
    mesh->addBuffer(uvCoords, 2, "in_Uv");
    mesh->addElementBuffer(indicies);
    mesh->setDrawType(GL_TRIANGLES);
    return mesh;
}

Mesh * Geometry::makePlaneTess() {
    vector<GLfloat> vertices = {
            1, 1, 0,
            1, -1, 0,
            -1, -1, 0,
            -1, 1, 0
    };

    vector<GLuint> indicies = { 0, 1, 2, 0, 2, 3 };
    Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices, 3, "in_Vertex");
    mesh->addElementBuffer(indicies);
    mesh->setDrawType(GL_PATCHES);
    return mesh;
}

Mesh * Geometry::makeTetrahedron() {
    vector<GLfloat> vertices = {
            1.0, 1.0, 1.0,
            -1.0, -1.0, 1.0,
            -1.0, 1.0, -1.0,
            1.0, -1.0, -1.0
    };

    vector<GLfloat> vertexColors = {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 1.0, 1.0
    };

    vector<GLfloat> uvCoords = {
            1.0, 0.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 1.0
    };

    vector<GLfloat> normals = {
            1.0, 1.0, 1.0,
            -1.0, -1.0, 1.0,
            -1.0, 1.0, -1.0,
            1.0, -1.0, -1.0 };

    vector<GLuint> indicies = { 0, 1, 2, 3, 0, 1 };

    Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices, 3, "in_Vertex");
    mesh->addBuffer(vertexColors, 3, "in_Color");
    mesh->addBuffer(normals, 3, "in_Normal");
    mesh->addBuffer(uvCoords, 2, "in_Uv");

    mesh->addElementBuffer(indicies);
    return mesh;
}

Mesh * Geometry::makeCube() {
    vector<GLfloat> vertices = {
        1.0, -1.0, -1.0,
        1.0, -1.0, 1.0,
        -1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0,
        1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0
    };

    vector<GLfloat> vertexColors = {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 1.0, 1.0,
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 1.0, 1.0
    };

    vector<GLfloat> uvCoords = {
        1.0, 0.0,
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0
    };

    vector<GLuint> indicies = {
            4, 0, 3, 4, 3, 7,
            1, 5, 2, 5, 6, 2,
            2, 6, 7, 2, 7, 3,
            0, 4, 1, 4, 5, 1,
            4, 7, 5, 7, 6, 5,
            0, 1, 2, 0, 2, 3
    };

    Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices, 3, "in_Vertex");
    mesh->addBuffer(vertices, 3, "in_Color");
    mesh->addBuffer(uvCoords, 2, "in_Uv");
    mesh->addBuffer(vertices, 3, "in_Normal");
    mesh->addElementBuffer(indicies);
    return mesh;
}

float Geometry::randomize(float density, float randomness) {
    unsigned seed;
    float randomValue = 1 / static_cast<float>((rand_r(&seed) % 20) + 1);

    return density + (density * randomValue * randomness);
}

Mesh * Geometry::makeStars(vector<float> & resolution, float density,
        float randomness, float colorIntensity) {
    vector<GLfloat> vertices, colors;
    vector<GLuint> indicies;

    unsigned i = 0;

    srand(time(NULL));

    for (float x = 0; x < resolution.at(0);
            x += randomize(density, randomness)) {
        for (float y = 0; y < resolution.at(1); y += randomize(density,
                randomness)) {
            for (float z = 0; z < resolution.at(2); z += randomize(density,
                    randomness)) {
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                colors.push_back(static_cast<float>(
                        static_cast<int>(x * colorIntensity) % 255) / 256);
                colors.push_back(static_cast<float>(
                        static_cast<int>(y * colorIntensity) % 255) / 256);
                colors.push_back(static_cast<float>(
                        static_cast<int>(z * colorIntensity) % 255) / 256);

                indicies.push_back(i);

                i++;
            }
        }
    }
    Logger::Instance().message << i << " Generated Stars";
    Logger::Instance().log("MESSAGE", "makeStars");

    Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices, 3, "in_Vertex");
    mesh->addBuffer(colors, 3, "in_Color");
    mesh->addElementBuffer(indicies);
    return mesh;
}

Mesh * Geometry::makeSpiral(int resolution, float density, float randomness) {
    vector<GLfloat> vertices, colors;
    vector<GLuint> indicies;

    QVector3D point = QVector3D(1, 0, 0);
    QMatrix4x4 rotation = QMatrix4x4();
    rotation.rotate(0.5, 0.0, 1.0, 0.0);

    QMatrix4x4 rotation2 = QMatrix4x4();
    rotation2.rotate(0.5, 0.0, 0.0, 1.0);

    unsigned i = 1;

    srand(time(NULL));
    for (i; i < resolution; i++) {
        QVector3D tempPoint = point.normalized();
        point += (tempPoint / 20.0);

        point = rotation * point;

        QVector3D newPoint = point;
        for (int j = 0; j < 100; j++) {
            newPoint = rotation2 * newPoint;

            QVector3D tempPoint2 = newPoint.normalized();
            newPoint += (tempPoint2 / 20.0);

            vertices.push_back(newPoint.x());
            vertices.push_back(newPoint.y());
            vertices.push_back(newPoint.z());

            colors.push_back(1.0 - (static_cast<float>(
                    static_cast<int>(newPoint.x()) % 255) / 256));
            colors.push_back(1.0 - (static_cast<float>(
                    static_cast<int>(newPoint.y()) % 255) / 256));
            colors.push_back(1.0 - (static_cast<float>(
                    static_cast<int>(newPoint.z()) % 255) / 256));

            indicies.push_back(i);
            i++;
        }
    }

    Logger::Instance().message << i << " Generated Points";
    Logger::Instance().log("MESSAGE", "makeSpiral");

    Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices, 3, "in_Vertex");
    mesh->addBuffer(colors, 3, "in_Color");
    mesh->addElementBuffer(indicies);
    mesh->setDrawType(GL_POINTS);
    return mesh;
}
