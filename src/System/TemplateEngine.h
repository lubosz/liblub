/*
 * TemplateEngine.h
 *
 *  Created on: Apr 11, 2011
 *      Author: bmonkey
 */

#ifndef TEMPLATEENGINE_H_
#define TEMPLATEENGINE_H_

#include "common/Singleton.h"
#include <grantlee_core.h>
#include <string>
using std::string;

class TemplateEngine : public Singleton<TemplateEngine>{
public:
  TemplateEngine();
  virtual ~TemplateEngine();
  Grantlee::Engine *m_engine;
  QString render(const string& file);
  void print(const string& file);
  Grantlee::Context c;
};

#endif /* TEMPLATEENGINE_H_ */
