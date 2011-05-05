/*
 * TemplateEngine.cpp
 *
 *  Created on: Apr 11, 2011
 *      Author: bmonkey
 */

#include "System/TemplateEngine.h"
#include "System/Config.h"
#include <QDebug>
#include <iostream>

TemplateEngine::TemplateEngine() {
  m_engine = new Grantlee::Engine();
  Grantlee::FileSystemTemplateLoader::Ptr loader =
      Grantlee::FileSystemTemplateLoader::Ptr(
          new Grantlee::FileSystemTemplateLoader());
  string dir = Config::Instance().value<string> ("templateDir");
  loader->setTemplateDirs(QStringList() << QString::fromStdString(dir));
  m_engine->addTemplateLoader(loader);

  c.insert("precision", true);

  vector<int> glContext = Config::Instance().values<int>("GLcontext");

  QString version = QString::number(glContext[0]) +  QString::number(glContext[1]) + "0 core";

  c.insert("version", version);
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
