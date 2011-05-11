#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>

class QSlider;
class GLWidget;
class QVBoxLayout;
class QCheckBox;
class Planet;

class QtWindow: public QWidget {
  Q_OBJECT

public:
  QtWindow();

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *e);
  void executeKeys();
  QVBoxLayout * planetControls(Planet* planet);
  QVBoxLayout * createFloatElement(QString name, const char *target, int value, int from, int to, Planet* planet);
  QCheckBox * createBoolElement(QString name, const char *target, bool value, Planet* planet);

private:
  QList<int> pressedKeys;
  GLWidget *glWidget;
};

#endif
