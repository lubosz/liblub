#include <QtGui>
#include <QUiLoader>
#include "glWidget.h"
#include "Window.h"
#include "Scene/SceneData.h"
#include "System/Config.h"

QtWindow::QtWindow() {
  glWidget = new GLWidget;
  Config::Instance().load("config.xml");
//  QUiLoader uiLoader;
//  QFile uiFile("scripts/valueChanger.ui");
//  QWidget *valueChanger = uiLoader.load(&uiFile);
//  mainLayout->addWidget(valueChanger);

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addWidget(glWidget);

  QVBoxLayout *sliderBarLayout = new QVBoxLayout;
  sliderBarLayout->setContentsMargins(0,0,0,800);
  mainLayout->addLayout(sliderBarLayout);

  sliderBarLayout->addLayout(createFloatElement("Red",SLOT(setRed(int))));
  sliderBarLayout->addLayout(createFloatElement("Green",SLOT(setGreen(int))));
  sliderBarLayout->addLayout(createFloatElement("Blue",SLOT(setBlue(int))));

  sliderBarLayout->addWidget(createBoolElement("Attenuation", SLOT(setAttenuation(bool))));
  setLayout(mainLayout);

  setWindowTitle(tr("LibLub"));
}

QCheckBox * QtWindow::createBoolElement(QString name, const char *target) {
  QCheckBox *checkBox = new QCheckBox();
  checkBox->setText(name);
  connect(checkBox, SIGNAL(clicked(bool)), glWidget->earth, target);
  connect(checkBox, SIGNAL(clicked(bool)), glWidget, SLOT(updateGL()));
  return checkBox;
}

QVBoxLayout * QtWindow::createFloatElement(QString name, const char *target) {
  QVBoxLayout *sliderLayout = new QVBoxLayout;
  QHBoxLayout *textAndValueLayout = new QHBoxLayout;
  QLabel * label = new QLabel(name);
  textAndValueLayout->addWidget(label);
  QSpinBox *spinBox = new QSpinBox();
  textAndValueLayout->addWidget(spinBox);
  sliderLayout->addLayout(textAndValueLayout);
  QSlider *slider = new QSlider(Qt::Horizontal);
  slider->setRange(0, 256);
  spinBox->setRange(0, 256);
  sliderLayout->addWidget(slider);
  connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
  connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
  connect(slider, SIGNAL(valueChanged(int)), glWidget->earth, target);
  connect(slider, SIGNAL(valueChanged(int)), glWidget, SLOT(updateGL()));
  return sliderLayout;
}

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

