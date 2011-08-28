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
class QGroupBox;
class QVector3D;

class PlanetWidget : public QWidget {
  Q_OBJECT
public:
  PlanetWidget(Planet* planet);
  virtual ~PlanetWidget();

  QVBoxLayout * createIntElement(QString name, const char *target, int value, int from, int to, Planet* planet);
  QCheckBox * createBoolElement(QString name, const char *target, bool value, Planet* planet);
  QGroupBox * tripleFloatGroup(const QString & title,
      Planet* planet, const QVector3D & defaultValues, float from, float to,
      QList<const char*> targets, QStringList names);

signals:
    void draw();
};

#endif /* PLANETWIDGET_H_ */
