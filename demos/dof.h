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

#include "Load/SceneLoader.h"
#include "Renderer/RenderPasses.h"
#include "System/Demo.h"

class DepthOfFieldDemo: public Demo {
public:
    SceneLoader *sceneLoader;

    ShaderProgram* pDebugDepthFBOMaterial;
    ShaderProgram* pDebugColorFBOMaterial;

    ShaderProgram* pDepthMaterial;
    ShaderProgram* pDOFShader;

    FrameBuffer* pFBOColor;
    FrameBuffer* pFBODepth;

    Mesh * fullPlane;

    QSize res;

    DepthOfFieldDemo();

    ~DepthOfFieldDemo();

    void init();
    void draw();
};


