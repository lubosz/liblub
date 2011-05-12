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
#include <QComboBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QGroupBox>
#include "Planets/Planet.h"
#include "FloatEditorWidget.h"

PlanetWidget::PlanetWidget(Planet* planet) {
  QVBoxLayout *sliderBarLayout = new QVBoxLayout(this);

  QTabWidget * tabWidget = new QTabWidget(this);
  sliderBarLayout->addWidget(tabWidget);
  QWidget * atmoTab = new QWidget();
  tabWidget->addTab(atmoTab, "Atmosphere");
  QWidget * planetTab = new QWidget();
  tabWidget->addTab(planetTab, "Planet");

  QVBoxLayout *planetTabLayout = new QVBoxLayout(planetTab);
  QVBoxLayout *atmoTabLayout = new QVBoxLayout(atmoTab);
  planetTabLayout->setContentsMargins(0, 0, 0, 200);
  atmoTabLayout->setContentsMargins(0, 0, 0, 200);

  atmoTabLayout->addWidget(
        tripleFloatGroup(
            "Light Wavelength [nm]",
            planet, planet->lightWavelength, 0.20, 1.5,
            QList<const char*>() << SLOT(setRed(double)) << SLOT(setGreen(double)) << SLOT(setBlue(double)),
            QStringList() << "Red" << "Green" << "Blue"
        )
    );

  planetTabLayout->addWidget(
        tripleFloatGroup(
            "Position",
            planet, planet->position, -50, 50,
            QList<const char*>() << SLOT(setX(double)) << SLOT(setY(double)) << SLOT(setZ(double)),
            QStringList() << "X" << "Y" << "Z"
        )
    );

  atmoTabLayout->addWidget(createBoolElement("Attenuation", SLOT(setAttenuation(bool)), planet->useAttenuation, planet));
  atmoTabLayout->addWidget(createBoolElement("Mie Phase", SLOT(setMie(bool)), planet->useMie, planet));
  atmoTabLayout->addWidget(createBoolElement("Rayleigh Phase", SLOT(setRayleigh(bool)), planet->useRayleigh, planet));

    QComboBox * comboBox = new QComboBox();

    comboBox->insertItems(0, QStringList() << "Sun" << "Ocean" << "TerrainPlain" << "TerrainTess");
    connect(comboBox, SIGNAL(activated(int)), planet, SLOT(setType(int)));
    connect(comboBox, SIGNAL(activated(int)), this, SIGNAL(updateGL()));
    comboBox->setCurrentIndex(planet->type);
    planetTabLayout->addWidget(comboBox);
}

QGroupBox * PlanetWidget::tripleFloatGroup(const QString & title,
    Planet* planet, const QVector3D & defaultValues, float from, float to,
    QList<const char*> targets, QStringList names) {
  QGroupBox * groupBox = new QGroupBox();
  groupBox->setTitle(title);

  FloatEditorWidget* red = new FloatEditorWidget(names[0],targets[0], defaultValues.x(), from, to, planet);
  FloatEditorWidget* green = new FloatEditorWidget(names[1],targets[1], defaultValues.y(), from, to, planet);
  FloatEditorWidget* blue = new FloatEditorWidget(names[2],targets[2], defaultValues.z(), from, to, planet);
  connect(red, SIGNAL(updateGL()), this, SIGNAL(updateGL()));
  connect(green, SIGNAL(updateGL()), this, SIGNAL(updateGL()));
  connect(blue, SIGNAL(updateGL()), this, SIGNAL(updateGL()));

  QVBoxLayout *groupBoxLayout = new QVBoxLayout(groupBox);
  groupBoxLayout->addWidget(red);
  groupBoxLayout->addWidget(green);
  groupBoxLayout->addWidget(blue);
  return groupBox;
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
