/*
 * FloatEditorWidget.cpp
 *
 *  Created on: May 11, 2011
 *      Author: bmonkey
 */

#include "FloatEditorWidget.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSlider>
#include "Planets/Planet.h"

FloatEditorWidget::FloatEditorWidget(QString name, const char *target, float value, float from, float to, Planet* planet) {
  QVBoxLayout *sliderLayout = new QVBoxLayout(this);
    QHBoxLayout *textAndValueLayout = new QHBoxLayout;
    QLabel * label = new QLabel(name);
    textAndValueLayout->addWidget(label);
    spinBox = new QDoubleSpinBox();
    textAndValueLayout->addWidget(spinBox);
    sliderLayout->addLayout(textAndValueLayout);
    slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, int(to * 100));
    spinBox->setRange(from, to);
    slider->setValue(int(value * 100));
    spinBox->setValue(value);
    LogDebug << target << planet->getSize();
    spinBox->setSingleStep (0.01);
    sliderLayout->addWidget(slider);
    connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBox(double)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateFromSlider(int)));
    connect(spinBox, SIGNAL(valueChanged(double)), planet, target);
}

void FloatEditorWidget::updateFromSlider(int value) {
  spinBox->setValue(double(value)/100.0);
  emit updateGL();
}

void FloatEditorWidget::updateFromSpinBox(double value) {
  slider->setValue(int(value * 100));
  emit updateGL();
}

FloatEditorWidget::~FloatEditorWidget() {
  // TODO Auto-generated destructor stub
}
