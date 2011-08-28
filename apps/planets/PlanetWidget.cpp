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
#include "Planet.h"
#include "Window/Qt/FloatEditorWidget.h"

PlanetWidget::PlanetWidget(Planet* planet) {
  if(planet == NULL)
    LogFatal << "No planet! :(";

  QVBoxLayout *sliderBarLayout = new QVBoxLayout(this);

  QTabWidget * tabWidget = new QTabWidget(this);
  sliderBarLayout->addWidget(tabWidget);
  QWidget * atmoTab = new QWidget();
  tabWidget->addTab(atmoTab, "Atmosphere");
  QWidget * planetTab = new QWidget();
  tabWidget->addTab(planetTab, "Planet");

  QVBoxLayout *planetTabLayout = new QVBoxLayout(planetTab);
  QVBoxLayout *atmoTabLayout = new QVBoxLayout(atmoTab);
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
    QGroupBox * groupBox2 = new QGroupBox();
      groupBox2->setMaximumHeight(400);
      groupBox2->setTitle("Planet Size");
    QVBoxLayout *groupBoxLayout2 = new QVBoxLayout(groupBox2);
    FloatEditorWidget* depthScale = new FloatEditorWidget("Scale Depth", SLOT(setScaleDepth(double)), 0.25, 0.03, 3, planet);
    connect(depthScale, SIGNAL(draw()), this, SIGNAL(draw()));
    groupBoxLayout2->addWidget(depthScale);

    FloatEditorWidget* innerRadius = new FloatEditorWidget("Inner Radius", SLOT(setInnerRadius(double)), 10.00, 0, 100, planet);
    connect(innerRadius, SIGNAL(draw()), this, SIGNAL(draw()));
    groupBoxLayout2->addWidget(innerRadius);

    FloatEditorWidget* outerRadius = new FloatEditorWidget("Outer Radius", SLOT(setOuterRadius(double)), 10.25, 0, 100, planet);
    connect(outerRadius, SIGNAL(draw()), this, SIGNAL(draw()));
    groupBoxLayout2->addWidget(outerRadius);


    QGroupBox * groupBox = new QGroupBox();
    groupBox->setMaximumHeight(200);
    groupBox->setTitle("Atmosphere Options");
    QVBoxLayout *groupBoxLayout = new QVBoxLayout(groupBox);

    groupBoxLayout->addWidget(createBoolElement("Attenuation", SLOT(setAttenuation(bool)), planet->useAttenuation, planet));
    groupBoxLayout->addWidget(createBoolElement("Mie Phase", SLOT(setMie(bool)), planet->useMie, planet));
    groupBoxLayout->addWidget(createBoolElement("Rayleigh Phase", SLOT(setRayleigh(bool)), planet->useRayleigh, planet));
    atmoTabLayout->addWidget(groupBox);
    QComboBox * comboBox = new QComboBox();

    comboBox->insertItems(0, QStringList() << "Sun" << "Ocean" << "TerrainPlain" << "TerrainTess");
    connect(comboBox, SIGNAL(activated(int)), planet, SLOT(setType(int)));
    connect(comboBox, SIGNAL(activated(int)), this, SIGNAL(draw()));
    comboBox->setCurrentIndex(planet->type);
    planetTabLayout->addWidget(comboBox);
    planetTabLayout->addWidget(groupBox2);
}

QGroupBox * PlanetWidget::tripleFloatGroup(const QString & title,
    Planet* planet, const QVector3D & defaultValues, float from, float to,
    QList<const char*> targets, QStringList names) {
  QGroupBox * groupBox = new QGroupBox();
  groupBox->setMaximumHeight(300);
  groupBox->setTitle(title);

  FloatEditorWidget* red = new FloatEditorWidget(names[0],targets[0], defaultValues.x(), from, to, planet);
  FloatEditorWidget* green = new FloatEditorWidget(names[1],targets[1], defaultValues.y(), from, to, planet);
  FloatEditorWidget* blue = new FloatEditorWidget(names[2],targets[2], defaultValues.z(), from, to, planet);
  connect(red, SIGNAL(draw()), this, SIGNAL(draw()));
  connect(green, SIGNAL(draw()), this, SIGNAL(draw()));
  connect(blue, SIGNAL(draw()), this, SIGNAL(draw()));

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
  connect(checkBox, SIGNAL(clicked(bool)), this, SIGNAL(draw()));
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
