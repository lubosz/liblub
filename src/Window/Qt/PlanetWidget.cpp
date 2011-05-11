/*
 * PlanetWidget.cpp
 *
 *  Created on: May 11, 2011
 *      Author: bmonkey
 */

#include "PlanetWidget.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSlider>
#include "Planets/Planet.h"
#include "FloatEditorWidget.h"

PlanetWidget::PlanetWidget(Planet* planet) {
    QVBoxLayout *sliderBarLayout = new QVBoxLayout(this);
    sliderBarLayout->setContentsMargins(0,0,0,800);
    FloatEditorWidget* red = new FloatEditorWidget("Red",SLOT(setRed(double)), planet->lightWavelength.x(), 0, 1.0, planet);
    FloatEditorWidget* green = new FloatEditorWidget("Green",SLOT(setGreen(double)), planet->lightWavelength.y(), 0, 1.0, planet);
    FloatEditorWidget* blue = new FloatEditorWidget("Blue",SLOT(setBlue(double)), planet->lightWavelength.z(), 0, 1.0, planet);

    connect(red, SIGNAL(updateGL()), this, SIGNAL(updateGL()));
    connect(green, SIGNAL(updateGL()), this, SIGNAL(updateGL()));
    connect(blue, SIGNAL(updateGL()), this, SIGNAL(updateGL()));

    sliderBarLayout->addWidget(red);
    sliderBarLayout->addWidget(green);
    sliderBarLayout->addWidget(blue);
    sliderBarLayout->addWidget(createBoolElement("Attenuation", SLOT(setAttenuation(bool)), planet->attenuation, planet));
}

PlanetWidget::~PlanetWidget() {
}

QCheckBox * PlanetWidget::createBoolElement(QString name, const char *target, bool value, Planet* planet) {
  QCheckBox *checkBox = new QCheckBox();
  checkBox->setText(name);
  checkBox->setChecked(value);
  connect(checkBox, SIGNAL(clicked(bool)), planet, target);
  connect(checkBox, SIGNAL(clicked(bool)), this, SIGNAL(updateGL()));
  return checkBox;
}

QVBoxLayout * PlanetWidget::createIntElement(QString name, const char *target, int value, int from, int to, Planet* planet) {
  QVBoxLayout *sliderLayout = new QVBoxLayout;
  QHBoxLayout *textAndValueLayout = new QHBoxLayout;
  QLabel * label = new QLabel(name);
  textAndValueLayout->addWidget(label);
  QSpinBox *spinBox = new QSpinBox();
  textAndValueLayout->addWidget(spinBox);
  sliderLayout->addLayout(textAndValueLayout);
  QSlider *slider = new QSlider(Qt::Horizontal);
  slider->setRange(from, to);
  spinBox->setRange(from, to);
  slider->setValue(value);
  spinBox->setValue(value);
  sliderLayout->addWidget(slider);
  connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
  connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
  connect(slider, SIGNAL(valueChanged(int)), planet, target);
  return sliderLayout;
}
