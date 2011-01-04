/*
 * RecourceManager.h
 *
 *  Created on: Dec 20, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Singleton.h"

class RecourceManager : public Singleton<RecourceManager> {
 public:
	RecourceManager();
	virtual ~RecourceManager();
};

