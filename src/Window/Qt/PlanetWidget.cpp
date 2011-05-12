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
#include "Planets/Planet.h"
#include "FloatEditorWidget.h"

PlanetWidget::PlanetWidget(Planet* planet) {
    QVBoxLayout *sliderBarLayout = new QVBoxLayout(this);
    sliderBarLayout->setContentsMargins(0,0,0,800);
    FloatEditorWidget* red = new FloatEditorWidget("Red",SLOT(setRed(double)), planet->lightWavelength.x(), 0.20, 1.5, planet);
    FloatEditorWidget* green = new FloatEditorWidget("Green",SLOT(setGreen(double)), planet->lightWavelength.y(), 0.20, 1.5, planet);
    FloatEditorWidget* blue = new FloatEditorWidget("Blue",SLOT(setBlue(double)), planet->lightWavelength.z(), 0.20, 1.5, planet);

    connect(red, SIGNAL(updateGL()), this, SIGNAL(updateGL()));
    connect(green, SIGNAL(updateGL()), this, SIGNAL(updateGL()));
    connect(blue, SIGNAL(updateGL()), this, SIGNAL(updateGL()));

    FloatEditorWidget* posx = new FloatEditorWidget("X",SLOT(setX(double)), planet->position.x(), -50, 50, planet);
    FloatEditorWidget* posy = new FloatEditorWidget("Y",SLOT(setY(double)), planet->position.y(), -50, 50, planet);
    FloatEditorWidget* posz = new FloatEditorWidget("Z",SLOT(setZ(double)), planet->position.z(), -50, 50, planet);

    connect(posx, SIGNAL(updateGL()), this, SIGNAL(updateGL()));
    connect(posy, SIGNAL(updateGL()), this, SIGNAL(updateGL()));
    connect(posz, SIGNAL(updateGL()), this, SIGNAL(updateGL()));

    sliderBarLayout->addWidget(red);
    sliderBarLayout->addWidget(green);
    sliderBarLayout->addWidget(blue);

    sliderBarLayout->addWidget(posx);
    sliderBarLayout->addWidget(posy);
    sliderBarLayout->addWidget(posz);

    sliderBarLayout->addWidget(createBoolElement("Attenuation", SLOT(setAttenuation(bool)), planet->useAttenuation, planet));
    sliderBarLayout->addWidget(createBoolElement("Mie Phase", SLOT(setMie(bool)), planet->useMie, planet));
    sliderBarLayout->addWidget(createBoolElement("Rayleigh Phase", SLOT(setRayleigh(bool)), planet->useRayleigh, planet));

    QComboBox * comboBox = new QComboBox();

    comboBox->insertItems(0, QStringList() << "Sun" << "TerrainTess" << "Ocean" << "TerrainPlain");

    //    QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
    //    ___qlistwidgetitem->setText(QApplication::translate("MainWindow", "Sun", 0, QApplication::UnicodeUTF8));

    sliderBarLayout->addWidget(comboBox);
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

//void retranslateUi(QMainWindow *MainWindow) {
//    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
//
//    const bool __sortingEnabled = listWidget->isSortingEnabled();
//    listWidget->setSortingEnabled(false);
//    QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
//    ___qlistwidgetitem->setText(QApplication::translate("MainWindow", "Sun", 0, QApplication::UnicodeUTF8));
//    QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
//    ___qlistwidgetitem1->setText(QApplication::translate("MainWindow", "Earth", 0, QApplication::UnicodeUTF8));
//    QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
//    ___qlistwidgetitem2->setText(QApplication::translate("MainWindow", "Ocean", 0, QApplication::UnicodeUTF8));
//    listWidget->setSortingEnabled(__sortingEnabled);
//
//}
