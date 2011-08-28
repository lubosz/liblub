#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>

class GLWidget;
class PlanetWidget;
class QHBoxLayout;

class PlanetWindow: public QWidget {
  Q_OBJECT

public:
  PlanetWindow();
  QHBoxLayout *mainLayout;

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *e);
  void executeKeys();

private:
  QList<int> pressedKeys;

signals:
  void draw();

};

#endif
