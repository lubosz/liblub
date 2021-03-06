/*
 * TemplateEngine.cpp
 *
 *  Created on: Apr 11, 2011
 *      Author: bmonkey
 */

#include "System/TemplateEngine.h"
#include "System/Config.h"
#include "System/Logger.h"
#include <QDebug>
#include <iostream>

TemplateEngine::TemplateEngine() {
  m_engine = new Grantlee::Engine();
  Grantlee::FileSystemTemplateLoader::Ptr loader =
      Grantlee::FileSystemTemplateLoader::Ptr(
          new Grantlee::FileSystemTemplateLoader());
  string dir = Config::Instance().value<string> ("shaderDir");
  loader->setTemplateDirs(QStringList() << QString::fromStdString(dir));
  m_engine->addTemplateLoader(loader);

  c.insert("precision", false);

  vector<int> GLcontext = Config::Instance().values<int>("GLcontext");
  bool useUniformBuffers = (GLcontext[0] == 3 && GLcontext[1] >= 1) || GLcontext[0] > 3;
  c.insert("useUniformBuffers", useUniformBuffers);

  vector<int> glContext = Config::Instance().values<int>("GLcontext");

  QString version;

  if (glContext[0] == 3 && glContext[1] < 3) {
      switch(glContext[1]) {
      case 0:
          version = "130";
          break;
      case 1:
          version = "140";
          break;
      case 2:
          version = "150";
          break;
      default:
          break;
      }
  } else if(glContext[0] >= 3) {
    version = QString::number(glContext[0]) +  QString::number(glContext[1]) + "0 core";
  } else {
    LogFatal << "Unsupported GL Version" << glContext[0] << "." << glContext[1];
  }

  c.insert("version", version);
}

TemplateEngine::~TemplateEngine() {
  delete m_engine;
}

void TemplateEngine::addFlag(const string& flag) {
    c.insert(QString::fromStdString(flag), QVariant(true));
}

void TemplateEngine::addFlags(const vector<string> & flags) {
    foreach (string flag, flags)
        addFlag(flag);
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

  LogDebug << content.toStdString();

  return content;
}

void TemplateEngine::print(const string& file) {
  qDebug() << render(file);
}
