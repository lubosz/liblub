/*
 * TemplateEngine.h
 *
 *  Created on: Apr 11, 2011
 *      Author: bmonkey
 */

#ifndef TEMPLATEENGINE_H_
#define TEMPLATEENGINE_H_

#include "Common/Singleton.h"
#include <grantlee_core.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

class TemplateEngine : public Singleton<TemplateEngine>{
public:
  TemplateEngine();
  virtual ~TemplateEngine();
  Grantlee::Engine *m_engine;
  QString render(const string& file);
  void print(const string& file);
  void addFlag(const string &flag);
  void addFlags(const vector<string> & flags);
  Grantlee::Context c;
};

#endif /* TEMPLATEENGINE_H_ */
