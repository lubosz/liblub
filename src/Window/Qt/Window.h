#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>

class GLWidget;

class QtWindow: public QWidget {
  Q_OBJECT

public:
  QtWindow();

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *e);
  void executeKeys();
//  QWidget *  planetControls(Planet* planet);

private:
  QList<int> pressedKeys;
  GLWidget *glWidget;
};

#endif
