#include <QtGui>
#include <QUiLoader>
#include "glWidget.h"
#include "Window.h"
#include "Scene/SceneData.h"

QtWindow::QtWindow() {
  glWidget = new GLWidget;

//  xSlider = createSlider();
//  ySlider = createSlider();
//  zSlider = createSlider();

//  QUiLoader uiLoader;
//  QFile uiFile("scripts/valueChanger.ui");
//  QWidget *valueChanger = uiLoader.load(&uiFile);

//  connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
//  connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
//  connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
//  connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
//  connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));
//  connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addWidget(glWidget);

  //bar hack
  QVBoxLayout *sliderBarLayout = new QVBoxLayout;
  sliderBarLayout->setContentsMargins(0,0,0,800);
  mainLayout->addLayout(sliderBarLayout);

  sliderBarLayout->addLayout(createControlElement("Red",SLOT(setRed(int))));
  sliderBarLayout->addLayout(createControlElement("Green",SLOT(setGreen(int))));
  sliderBarLayout->addLayout(createControlElement("Blue",SLOT(setBlue(int))));
//  mainLayout->addWidget(valueChanger);
  setLayout(mainLayout);

//  xSlider->setValue(15 * 16);
//  ySlider->setValue(345 * 16);
//  zSlider->setValue(0 * 16);
  setWindowTitle(tr("LibLub"));
}

QVBoxLayout * QtWindow::createControlElement(QString name, const char *targetColor) {
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

QSlider *QtWindow::createSlider() {
  QSlider *slider = new QSlider(Qt::Horizontal);
//  slider->setRange(0, 360);
//  slider->setSingleStep(16);
//  slider->setPageStep(15 * 16);
//  slider->setTickInterval(15 * 16);
//  slider->setTickPosition(QSlider::TicksRight);
  return slider;
}

void QtWindow::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Escape)
    close();
  else
    pressedKeys.push_back(e->key());

  //    QWidget::keyPressEvent(e);
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

