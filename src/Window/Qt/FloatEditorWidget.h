/*
 * FloatEditorWidget.h
 *
 *  Created on: May 11, 2011
 *      Author: bmonkey
 */

#ifndef FLOATEDITORWIDGET_H_
#define FLOATEDITORWIDGET_H_
#include <QWidget>
class Planet;
class QDoubleSpinBox;
class QSlider;

class FloatEditorWidget  : public QWidget {
  Q_OBJECT
public:
  FloatEditorWidget(QString name, const char *targetSlot, float value, float from, float to, QObject* targetObject);
  virtual ~FloatEditorWidget();
private:
  QDoubleSpinBox * spinBox;
  QSlider * slider;

public slots:
  void updateFromSlider(int value);
  void updateFromSpinBox(double value);

signals:
  void draw();
};

#endif /* FLOATEDITORWIDGET_H_ */
