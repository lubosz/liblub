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

  c.insert("precision", false);
  c.insert("version", "410 core");
}

TemplateEngine::~TemplateEngine() {
  // TODO Auto-generated destructor stub
}

QString TemplateEngine::render(const string& file) {
  Grantlee::Template t = m_engine->loadByName(QString::fromStdString(file));

  if (!t) {
    qDebug() << "Unable to load template"
        << QString("Error loading template: %1").arg(QString::fromStdString(file));
    return QString();
  }

  if (t->error()) {
    qDebug() << "Unable to load template"
        << QString("Error loading template: %1").arg(t->errorString());
    return QString();
  }

  QString content = t->render(&c);

  if (t->error()) {
    qDebug() << "Unable render template"
        << QString("Error rendering template: %1").arg(t->errorString());
    return QString();
  }
  content.replace("\n\n", "\n");

  return content;
}

void TemplateEngine::print(const string& file) {
  qDebug() << render(file);
}
