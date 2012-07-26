//
//  RenderingEngine1.cpp
//  OpenGL_ES
//
//  Created by khkim on 12. 7. 23..
//  Copyright (c) 2012년 chickenjuice.net. All rights reserved.
//

#include "IRenderingEngine.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include "Vector.hpp"
#include "Quaternion.hpp"
#include <vector>

static const float AnimationDuration = 0.25f;

using namespace std;

struct Vertex
{
	vec3 position;
	vec4 color;
};

struct Animation
{
	Quaternion start;
	Quaternion end;
	Quaternion current;
	
	float elapsed;
	float duration;
};

class RenderingEngine1 : public IRenderingEngine
{
public:
	RenderingEngine1();
	
	void initialize(int width, int height);
	void render() const;
	void updateAnimation(float timeStep);
	void onRotate(DeviceOrientation orientation);
	
private:
	vector<Vertex> _cone;
	vector<Vertex> _disk;
	Animation _animation;
	
	GLuint _frameBuffer;
	GLuint _renderBuffer;
	GLuint _depthBuffer;
};

IRenderingEngine *createRenderer1()
{
	return new RenderingEngine1();
}

RenderingEngine1::RenderingEngine1()
{
	glGenRenderbuffersOES(1, &_renderBuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, _renderBuffer);
}

void RenderingEngine1::initialize(int width, int height)
{
	const float coneRadius = 0.5f;
	const float coneHeight = 1.866f;
	const int coneSlices = 40;
	
	// Create cone
	{
		_cone.resize((coneSlices + 1) * 2);
		
		const float dtheta = TwoPi / coneSlices;
		float theta = 0;
		
		for (vector<Vertex>::iterator vertex = _cone.begin(); vertex != _cone.end(); ++vertex)
		{
			float brightness = abs(sin(theta));
			vec4 color(brightness, brightness, brightness, 1);
			
			vertex->position = vec3(0, 1, 0);
			vertex->color = color;
			++vertex;
			
			vertex->position.x = coneRadius * cos(theta);
			vertex->position.y = 1 - coneHeight;
			vertex->position.z = coneRadius * sin(theta);
			vertex->color = color;
			
			theta += dtheta;
		}
	}
	
	// Create disk
	{
		_disk.resize(coneSlices + 2);
		
		vector<Vertex>::iterator vertex = _disk.begin();
		vertex->color = vec4(0.75f, 0.75f, 0.75f, 1);
		vertex->position.x = 0;
		vertex->position.y = 1 - coneHeight;
		vertex->position.z = 0;
		vertex++;
		
		const float dtheta = TwoPi / coneSlices;
		float theta = 0;
		
		for ( ; vertex != _disk.end(); ++vertex)
		{
			vertex->color = vec4(0.75f, 0.75f, 0.75f, 1);
			vertex->position.x = coneRadius * cos(theta);
			vertex->position.y = 1 - coneHeight;
			vertex->position.z = coneRadius * sin(theta);
			
			theta += dtheta;
		}
	}
	
	// Create depth buffer
	glGenRenderbuffersOES(1, &_depthBuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, _depthBuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, width, height);

	// Create frame buffer
	glGenFramebuffersOES(1, &_frameBuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, _frameBuffer);
	
	// Connect render buffer and depth buffer to frame buffer
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, _renderBuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, _depthBuffer);
	
	// Bind render buffer for rendering
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, _renderBuffer);
	
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glFrustumf(-1.6f, 1.6f, -2.4f, 2.4f, 5, 10);

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0, 0, -7);
}

void RenderingEngine1::render() const
{
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	mat4 rotation(_animation.current.toMatrix());
	glMultMatrixf(rotation.pointer());

	// Draw cone
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &_cone[0].position.x);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &_cone[0].color.x);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, _cone.size());
	
	// Draw disk
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &_disk[0].position.x);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &_disk[0].color.x);
	glDrawArrays(GL_TRIANGLE_FAN, 0, _disk.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glPopMatrix();
}

void RenderingEngine1::updateAnimation(float timeStep)
{
	if (_animation.current == _animation.end)
		return;
	
	_animation.elapsed += timeStep;
	if (_animation.elapsed >= AnimationDuration)
	{
		_animation.current = _animation.end;
	}
	else 
	{
		float mu = _animation.elapsed / AnimationDuration;
		_animation.current = _animation.start.slerp(mu, _animation.end);
	}
}

void RenderingEngine1::onRotate(DeviceOrientation orientation)
{
	vec3 direction;
	
	switch (orientation) {
		case DeviceOrientationUnknown:
		case DeviceOrientationPortrait:
			direction = vec3(0, 1, 0);
			break;
			
		case DeviceOrientationPortraitUpsideDown:
			direction = vec3(0, -1, 0);
			break;

		case DeviceOrientationFaceDown:
			direction = vec3(0, 0, -1);
			break;

		case DeviceOrientationFaceUp:
			direction = vec3(0, 0, 1);
			break;
			
		case DeviceOrientationLandscapeLeft:
			direction = vec3(1, 0, 0);
			break;
			
		case DeviceOrientationLandscapeRight:
			direction = vec3(-1, 0, 0);
			break;
	}

	_animation.elapsed = 0;
	_animation.start = _animation.current = _animation.end;
	_animation.end = Quaternion::createFromVectors(vec3(0, 1, 0), direction);
}