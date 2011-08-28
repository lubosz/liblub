#include <QtGui>
#include <QUiLoader>
#include "glWidget.h"
#include "PlanetWindow.h"
#include "Scene/SceneData.h"
#include "System/Config.h"
#include "PlanetWidget.h"
#include "FloatEditorWidget.h"

PlanetWindow::PlanetWindow() {
  mainLayout = new QHBoxLayout;
  setLayout(mainLayout);
}

void PlanetWindow::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Escape)
    close();
  else
    pressedKeys.push_back(e->key());

  executeKeys();
}

void PlanetWindow::executeKeys() {
  float inputSpeed = .1;
  foreach(int key, pressedKeys) {
      if (key == Qt::Key_W)
        SceneData::Instance().getCurrentCamera()->forwardDirection(inputSpeed);
      if (key == Qt::Key_A)
        SceneData::Instance().getCurrentCamera()->leftDirection(inputSpeed);
      if (key == Qt::Key_S)
        SceneData::Instance().getCurrentCamera()->backwardDirection(inputSpeed);
      if (key == Qt::Key_D)
        SceneData::Instance().getCurrentCamera()->rightDirection(inputSpeed);
  }
  emit draw();
}

void PlanetWindow::keyReleaseEvent(QKeyEvent *e) {
  pressedKeys.removeAll(e->key());
}

