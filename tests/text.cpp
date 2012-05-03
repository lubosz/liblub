#include <cppunit/TestCase.h>

#include "Material/Textures.h"
#include "Material/ProcTextures.h"
#include "System/Config.h"

class TextureTest: public CppUnit::TestCase {
public:
  TextureTest(std::string name) :
    CppUnit::TestCase(name) {
  }

  void runTest() {
    QImage image(800,800);
    QPainter fontPainter(image);
    //draw font
    fontPainter.setRenderHint(QPainter::Antialiasing, true);
    fontPainter.setRenderHint(QPainter::TextAntialiasing, true);
    fontPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    fontPainter.setFont(QFont("Ubuntu", 24));
    fontPainter.setLayoutDirection(Qt::RightToLeft);
    fontPainter.setPen(Qt::white);

    string text = "fooo";

    fontPainter.drawText(drawBox, Qt::AlignLeft, QString::fromStdString(text));
    fontPainter.end();

  }
};

int main() {
  TextureTest foo2("bar");
  foo2.runTest();
}






