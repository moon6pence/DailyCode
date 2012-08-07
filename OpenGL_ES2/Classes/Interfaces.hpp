//
//  Interfaces.hpp
//  OpenGL_ES2
//
//  Created by khkim on 8/8/12.
//
//

#pragma once

#include "Vector.hpp"
#include "Quaternion.hpp"
#include <vector>

using std::vector;

struct ISurface
{
	virtual ~ISurface() { }
	
	virtual int getVertexCount() const = 0;
	virtual int getLineIndexCount() const = 0;

	virtual void generateVertices(vector<float>& vertices) const = 0;
	virtual void generateLineIndices(vector<unsigned short>& indices) const = 0;
};

struct Visual
{
	vec3 color;
	ivec2 lowerLeft;
	ivec2 viewportSize;
	Quaternion orientation;
};

struct IRenderingEngine
{
	static IRenderingEngine *createRenderingEngine();
	virtual ~IRenderingEngine() { }
	
	virtual void initialize(const vector<ISurface *>& surfaces) = 0;
	virtual void render(const vector<Visual>& visuals) const = 0;
};

struct IApplicationEngine
{
	static IApplicationEngine *createApplicationEngine(IRenderingEngine *renderingEngine);
	virtual ~IApplicationEngine() { }
	
	virtual void initialize(int width, int height) = 0;
	virtual void render() const = 0;
	virtual void updateAnimation(float timeStep) = 0;
	virtual void onMouseUp(ivec2 location) = 0;
	virtual void onMouseDown(ivec2 location) = 0;
	virtual void onMouseMove(ivec2 oldLocation, ivec2 newLocation) = 0;
};
