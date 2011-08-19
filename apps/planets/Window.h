#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>

class GLWidget;
class PlanetWidget;

class QtWindow: public QWidget {
  Q_OBJECT

public:
  QtWindow();

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *e);
  void executeKeys();
  PlanetWidget *  focusPlanet();

private:
  GLWidget *glWidget;
  QList<int> pressedKeys;
};

#endif
