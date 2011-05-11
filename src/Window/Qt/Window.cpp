#include <QtGui>
#include <QUiLoader>
#include "glWidget.h"
#include "Window.h"
#include "Scene/SceneData.h"

QtWindow::QtWindow() {
  glWidget = new GLWidget;

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

  QCheckBox *checkBox = new QCheckBox();
  checkBox->setText("Attenuation");
  sliderBarLayout->addWidget(checkBox);
  connect(checkBox, SIGNAL(valueChanged(bool)), glWidget, SLOT(setAttenuation(bool)));

  setLayout(mainLayout);

  setWindowTitle(tr("LibLub"));
}


QVBoxLayout * QtWindow::createFloatElement(QString name, const char *targetColor) {
  QVBoxLayout *sliderLayout = new QVBoxLayout;

  QHBoxLayout *textAndValueLayout = new QHBoxLayout;
  QLabel * label = new QLabel(name);
  textAndValueLayout->addWidget(label);
  QSpinBox *spinBox = new QSpinBox();
  textAndValueLayout->addWidget(spinBox);
  sliderLayout->addLayout(textAndValueLayout);
  QSlider *slider = new QSlider(Qt::Horizontal);
  slider->setRange(0, 256);
  spinBox->setRange(0,256);
  sliderLayout->addWidget(slider);
  connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
  connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
  connect(slider, SIGNAL(valueChanged(int)), glWidget, targetColor);
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

