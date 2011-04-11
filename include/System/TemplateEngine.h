/*
 * TemplateEngine.h
 *
 *  Created on: Apr 11, 2011
 *      Author: bmonkey
 */

#ifndef TEMPLATEENGINE_H_
#define TEMPLATEENGINE_H_

#include "common/Singleton.h"

class TemplateEngine : public Singleton<TemplateEngine>{
public:
  TemplateEngine();
  virtual ~TemplateEngine();
};

#endif /* TEMPLATEENGINE_H_ */
