#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>

class QSlider;
class GLWidget;
class QVBoxLayout;

class QtWindow: public QWidget {
  Q_OBJECT

public:
  QtWindow();

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *e);
  void executeKeys();
  QVBoxLayout * createFloatElement(QString name, const char *targetColor);

private:
  QList<int> pressedKeys;
  GLWidget *glWidget;
};

#endif
