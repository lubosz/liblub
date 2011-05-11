/*
 * PlanetWidget.h
 *
 *  Created on: May 11, 2011
 *      Author: bmonkey
 */

#ifndef PLANETWIDGET_H_
#define PLANETWIDGET_H_

#include <QWidget>
class QSlider;
class QVBoxLayout;
class QCheckBox;
class Planet;

class PlanetWidget : public QWidget {
  Q_OBJECT
public:
  PlanetWidget(Planet* planet);
  virtual ~PlanetWidget();

  QVBoxLayout * createIntElement(QString name, const char *target, int value, int from, int to, Planet* planet);
  QCheckBox * createBoolElement(QString name, const char *target, bool value, Planet* planet);

signals:
    void updateGL();
};

#endif /* PLANETWIDGET_H_ */
