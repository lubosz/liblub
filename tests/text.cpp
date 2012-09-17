#include <cppunit/TestCase.h>

#include "Texture/Textures.h"
#include "Procedural/ProcTextures.h"
#include "System/Config.h"
#include <QPainter>
#include <QApplication>

class TextureTest: public CppUnit::TestCase {
public:
  TextureTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {

    QSize screenSize = QSize(480,600);
    QImage image = QImage(screenSize, QImage::Format_ARGB32);
    QRectF drawBox = QRectF(50,200,screenSize.width()-100, screenSize.height()-100);

    QPainter fontPainter(&image);
    //draw font
    fontPainter.setRenderHint(QPainter::Antialiasing, true);
    fontPainter.setRenderHint(QPainter::TextAntialiasing, true);
    fontPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    fontPainter.setFont(QFont("Ubuntu", 50));
    fontPainter.setLayoutDirection(Qt::RightToLeft);
    fontPainter.setPen(Qt::black);

    string text = "foooBUNTU";
    fontPainter.drawText(drawBox, Qt::AlignLeft, QString::fromStdString(text));
    fontPainter.end();

    image.save("texttest.png");

  }
};

//int main() {
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  TextureTest foo2("bar");
  foo2.runTest();
}

