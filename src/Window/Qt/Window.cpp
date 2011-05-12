#include <QtGui>
#include <QUiLoader>
#include "glWidget.h"
#include "Window.h"
#include "Scene/SceneData.h"
#include "System/Config.h"
#include "PlanetWidget.h"

QtWindow::QtWindow() {
  glWidget = new GLWidget;
  Config::Instance().load("config.xml");
//  QUiLoader uiLoader;
//  QFile uiFile("scripts/valueChanger.ui");
//  QWidget *valueChanger = uiLoader.load(&uiFile);
//  mainLayout->addWidget(valueChanger);

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addWidget(glWidget);
  PlanetWidget * planetWidget = new PlanetWidget(glWidget->earth);
  connect(planetWidget, SIGNAL(updateGL(void)), glWidget, SLOT(updateGL(void)));

  QVBoxLayout *sideLayout = new QVBoxLayout;



  QListWidget * planetList = new QListWidget();

  foreach(Planet * planet , glWidget->planets){
    QListWidgetItem * planetItem = new QListWidgetItem(planetList);
    planetItem->setText(planet->name);
  }
  sideLayout->addWidget(planetList);
  sideLayout->addWidget(planetWidget);
  mainLayout->addLayout(sideLayout);

  setLayout(mainLayout);

  setWindowTitle(tr("LibLub"));
  glWidget->setFocus();
}

//QWidget * QtWindow::planetControls(Planet* planet) {
//  QWidget * pcontrol = new QWidget();
////  pcontrol->addLayout(sliderBarLayout);
//  return pcontrol;
//}

void QtWindow::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Escape)
    close();
  else
    pressedKeys.push_back(e->key());

  executeKeys();
}

void QtWindow::executeKeys() {
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
  glWidget->updateGL();
}

void QtWindow::keyReleaseEvent(QKeyEvent *e) {
  pressedKeys.removeAll(e->key());
}

