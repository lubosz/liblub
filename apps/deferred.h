/*
 Copyright © 2010 Lubosz Sarnecki

 This file is part of liblub.

 liblub is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 liblub is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with liblub.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "System/Application.h"
#include "Scene/SceneLoader.h"
#include "Renderer/RenderPasses.h"

class DeferredLightApp: public Application {
  Q_OBJECT
public:
  SceneLoader *sceneLoader;
  vector<DrawThing*> drawPasses;
  vector<ShadowCastPass*> shadowCastPasses;

  struct LightSource {
    QVector4D position;
    QVector4D diffuse;
    QVector4D specular;
    QVector4D direction;
  };

  LightSource lightBufferData[10];
  UniformBuffer * lightBuffer;
  GLint lightBufferSize;

  DeferredLightApp(int argc, char *argv[]);

  ~DeferredLightApp();

  void scene();
  void renderFrame();
  void initLightBuffer(ShaderProgram * shader, const string& bufferName);
  void initWidgets(QHBoxLayout * mainLayout);
public slots:
  void setOffSetFactor(double factor);
  void setOffSetUnits(double units);
};
