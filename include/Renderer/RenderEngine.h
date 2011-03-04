/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderEngine.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "Scene/SceneGraph.h"
#include "Renderer/RenderSequence.h"

class RenderSequence;

class RenderEngine : public Singleton<RenderEngine> {
 public:
	bool useFBO;
	bool lightView;
	bool wire;
	void display();
	void toggleFBO();
	void toggleWire();
	void clear();
	void toggleLightView();
	RenderSequence * shadowSequence;
	void setClearColor(const QVector3D & backgroundColor);

 private:
	friend class Singleton<RenderEngine>;

	unsigned frameCount;
	void checkVersion();
	GLboolean QueryExtension(char *extName);

	RenderEngine();
	virtual ~RenderEngine();
};
