/*
 * Copyright © 2010 Lubosz Sarnecki
 * RecourceManager.h
 *
 *  Created on: Dec 20, 2010
 */

#pragma once

#include "common/Singleton.h"

class RecourceManager : public Singleton<RecourceManager> {
 public:
	RecourceManager();
	virtual ~RecourceManager();
};

