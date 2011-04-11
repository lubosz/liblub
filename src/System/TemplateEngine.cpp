/*
 * TemplateEngine.cpp
 *
 *  Created on: Apr 11, 2011
 *      Author: bmonkey
 */

#include "System/TemplateEngine.h"
#include <QDebug>
#include <iostream>

TemplateEngine::TemplateEngine() {
  m_engine = new Grantlee::Engine();
  Grantlee::FileSystemTemplateLoader::Ptr loader =
      Grantlee::FileSystemTemplateLoader::Ptr(
          new Grantlee::FileSystemTemplateLoader());
  loader->setTemplateDirs(QStringList() << "templates");
  m_engine->addTemplateLoader(loader);

}

TemplateEngine::~TemplateEngine() {
  // TODO Auto-generated destructor stub
}

void TemplateEngine::render(const string& file) {

  Grantlee::Context c;
  c.insert("precision", false);
  c.insert( "version", "lolversion" );


  Grantlee::Template t = m_engine->loadByName(QString::fromStdString(file));

  if (!t) {
    qDebug() << "Unable to load template"
        << QString("Error loading template: %1").arg(QString::fromStdString(file));
    return;
  }

  if (t->error()) {
    qDebug() << "Unable to load template"
        << QString("Error loading template: %1").arg(t->errorString());
    return;
  }

  QString content = t->render(&c);

  if (t->error()) {
    qDebug() << "Unable render template"
        << QString("Error rendering template: %1").arg(t->errorString());
    return;
  }

  std::cout << qPrintable(content);
}
