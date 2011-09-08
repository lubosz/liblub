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
#include "Scene/SceneLoader.h"
#include "Scene/SceneGraph.h"
#include "Renderer/RenderPasses.h"
#include "System/Application.h"
#include <QVariantAnimation>

class AnimationApp: public Application {
public:

  SceneLoader *sceneLoader;

  AnimationApp(int argc, char *argv[]) :
    Application(argc, argv) {
    sceneLoader = new SceneLoader("nice.xml");
  }

  ~AnimationApp() {
    delete sceneLoader;
  }

  void scene() {
    sceneLoader->load();

    Node * sponge = SceneGraph::Instance().getNode("menger sponge");
    QPropertyAnimation * spongeSize = new QPropertyAnimation(sponge, "size");
    spongeSize->setDuration(20000);
    spongeSize->setKeyValueAt(0, 0.5f);
    spongeSize->setKeyValueAt(0.5, 5.0f);
    spongeSize->setKeyValueAt(1, 0.5f);
    spongeSize->setEasingCurve(QEasingCurve::InOutBounce);
    spongeSize->setLoopCount(-1);

    QPropertyAnimation * spongeRotation = new QPropertyAnimation(sponge,
        "rotationX");
    spongeRotation->setDuration(10000);
    spongeRotation->setStartValue(0);
    spongeRotation->setEndValue(360.0f);
    spongeRotation->setEasingCurve(QEasingCurve::InOutSine);
    spongeRotation->setLoopCount(-1);

    spongeSize->start();
    spongeRotation->start();

    Node * stars = SceneGraph::Instance().getNode("stars");
    QPropertyAnimation * starsAnim = new QPropertyAnimation(stars, "rotationX");
    starsAnim->setDuration(30000);
    starsAnim->setKeyValueAt(0, 0);
    starsAnim->setKeyValueAt(0.5, -100.0f);
    starsAnim->setKeyValueAt(1, 0);
    starsAnim->setEasingCurve(QEasingCurve::InOutCubic);
    starsAnim->setLoopCount(-1);

    int i = -10;
    foreach(Node * node, SceneGraph::Instance().sceneNodes) {
      if (node != sponge && node != stars) {
        QPropertyAnimation * posAnim = new QPropertyAnimation(node, "positionX");
        posAnim->setDuration(5000);
        posAnim->setKeyValueAt(0, node->positionX());
        posAnim->setKeyValueAt(0.5, node->positionX() - i * 2);
        posAnim->setKeyValueAt(1, node->positionX());
        posAnim->setEasingCurve(QEasingCurve::InOutElastic);
        posAnim->setLoopCount(-1);

        QPropertyAnimation * rotAnim = new QPropertyAnimation(node, "rotationZ");
        rotAnim->setDuration(5010);
        rotAnim->setKeyValueAt(0, node->positionX());
        rotAnim->setKeyValueAt(0.6, -(node->positionX() - i * 10));
        rotAnim->setKeyValueAt(1, node->positionX());
        rotAnim->setEasingCurve(QEasingCurve::InOutElastic);
        rotAnim->setLoopCount(-1);

        rotAnim->start();
        posAnim->start();
        i++;
      }
    }

    starsAnim->start();
    spongeSize->start();
    spongeRotation->start();
  }
  void renderFrame() {
      OnePass::draw();
  }
};

int main(int argc, char *argv[]) {
  AnimationApp(argc, argv).run();
}

