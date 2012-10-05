/*
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#include "QtInput.h"
#include "Scene/Scene.h"
#include "System/Config.h"

QtInput::QtInput() {
  inputSpeed = Config::Instance().value<float>("inputSpeedSlow");
  mouseSensitivity = Config::Instance().value<float>("mouseSensitivity");
  inputTimer = new QTimer(this);
  connect(inputTimer, SIGNAL(timeout()), this, SLOT(move()));
}

void QtInput::move() {

    if (pressedKeys.contains(Qt::Key_W))
        Scene::Instance().getCurrentCamera()->forwardDirection(inputSpeed);
    if (pressedKeys.contains(Qt::Key_S))
        Scene::Instance().getCurrentCamera()->backwardDirection(inputSpeed);
    if (pressedKeys.contains(Qt::Key_A))
        Scene::Instance().getCurrentCamera()->leftDirection(inputSpeed);
    if (pressedKeys.contains(Qt::Key_D))
        Scene::Instance().getCurrentCamera()->rightDirection(inputSpeed);
//    if (mousePosition != QVector2D(0,0))
    Scene::Instance().getCurrentCamera()->setMouseLook(mousePosition.x(), mousePosition.y(), mouseSensitivity);
    Scene::Instance().getCurrentCamera()->updateView();
    emit draw();
}

QtInput::~QtInput() {
	// TODO Auto-generated destructor stub
}
