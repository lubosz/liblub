#include <QtGui>

#include "glWidget.h"
#include "Window.h"
#include "Scene/SceneData.h"

QtWindow::QtWindow() {
  glWidget = new GLWidget;

  xSlider = createSlider();
  ySlider = createSlider();
  zSlider = createSlider();

  connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
  connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
  connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
  connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
  connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));
  connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addWidget(glWidget);
  mainLayout->addWidget(xSlider);
  mainLayout->addWidget(ySlider);
  mainLayout->addWidget(zSlider);
  setLayout(mainLayout);

  xSlider->setValue(15 * 16);
  ySlider->setValue(345 * 16);
  zSlider->setValue(0 * 16);
  setWindowTitle(tr("LibLub"));
}

QSlider *QtWindow::createSlider() {
  QSlider *slider = new QSlider(Qt::Vertical);
  slider->setRange(0, 360);
  slider->setSingleStep(16);
  slider->setPageStep(15 * 16);
  slider->setTickInterval(15 * 16);
  slider->setTickPosition(QSlider::TicksRight);
  return slider;
}

void QtWindow::keyPressEvent(QKeyEvent *e) {
  float inputSpeed = .1;
  if (e->key() == Qt::Key_Escape)
    close();
  else if (e->key() == Qt::Key_W){
    SceneData::Instance().getCurrentCamera()->forwardDirection(inputSpeed);
    glWidget->updateGL();
  }
  else if (e->key() == Qt::Key_A){
    SceneData::Instance().getCurrentCamera()->leftDirection(inputSpeed);
    glWidget->updateGL();
  }
  else if (e->key() == Qt::Key_S){
    SceneData::Instance().getCurrentCamera()->backwardDirection(inputSpeed);
    glWidget->updateGL();
  }
  else if (e->key() == Qt::Key_D){
    SceneData::Instance().getCurrentCamera()->rightDirection(inputSpeed);
    glWidget->updateGL();
  }
  else
    QWidget::keyPressEvent(e);
}
