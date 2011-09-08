/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderEngine.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "Scene/SceneGraph.h"
#include "common/Singleton.h"

class RenderEngine : public Singleton<RenderEngine> {
 public:
	bool useFBO;
	bool lightView;
	bool wire;
	void toggleFBO();
	void toggleWire();
	void setWire(bool wire);
	void clear();
	void toggleLightView();
	void setClearColor(const QVector3D & backgroundColor);
	void updateViewport(QSize& size);

 private:
	friend class Singleton<RenderEngine>;

	unsigned frameCount;
	void checkVersion();
	GLboolean QueryExtension(char *extName);

	RenderEngine();
	virtual ~RenderEngine();
};
