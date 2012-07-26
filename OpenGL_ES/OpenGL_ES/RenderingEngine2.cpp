//
//  RenderingEngine2.cpp
//  OpenGL_ES
//
//  Created by khkim on 12. 7. 23..
//  Copyright (c) 2012년 chickenjuice.net. All rights reserved.
//

#include "IRenderingEngine.h"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "Vector.hpp"
#include "Quaternion.hpp"
#include <vector>
#include <iostream>

#define STRINGFY(A) #A
#include "Simple.vert"
#include "Simple.frag"

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

class RenderingEngine2 : public IRenderingEngine
{
public:
	RenderingEngine2();
	
	void initialize(int width, int height);
	void render() const;
	void updateAnimation(float timeStep);
	void onRotate(DeviceOrientation orientation);
	
private:
	GLuint buildShader(const char *source, GLenum shaderType) const;
	GLuint buildProgram(const char *vertexShader, const char *fragmentShader) const;
	
	vector<Vertex> _cone;
	vector<Vertex> _disk;
	Animation _animation;

	GLuint _program;
	GLuint _frameBuffer;
	GLuint _renderBuffer;
	GLuint _depthBuffer;
};

IRenderingEngine *createRenderer2()
{
	return new RenderingEngine2();
}

RenderingEngine2::RenderingEngine2()
{
	glGenRenderbuffers(1, &_renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
}

void RenderingEngine2::initialize(int width, int height)
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
	glGenRenderbuffers(1, &_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	
	// Create frame buffer
	glGenFramebuffers(1, &_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	
	// Connect render buffer and depth buffer to frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
	
	// Bind render buffer for rendering
	glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
	
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	_program = buildProgram(SimpleVertexShader, SimpleFragmentShader);
	glUseProgram(_program);
	
	mat4 projectionMatrix = mat4::frustum(-1.6f, 1.6f, -2.4f, 2.4f, 5, 10);
	GLuint projectionUniform = glGetUniformLocation(_program, "Projection");
	glUniformMatrix4fv(projectionUniform, 1, 0, projectionMatrix.pointer());
}

void RenderingEngine2::render() const
{
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLuint positionSlot = glGetAttribLocation(_program, "Position");
	GLuint colorSlot = glGetAttribLocation(_program, "SourceColor");
	
	glEnableVertexAttribArray(positionSlot);
	glEnableVertexAttribArray(colorSlot);

	// Configure Model-view matrix
	mat4 rotation(_animation.current.toMatrix());
	mat4 translation = mat4::translate(0, 0, -7);
	mat4 modelviewMatrix = rotation * translation;

	GLuint modelviewUniform = glGetUniformLocation(_program, "Modelview");
	glUniformMatrix4fv(modelviewUniform, 1, 0, modelviewMatrix.pointer());

	// Draw cone
	{
		GLsizei stride = sizeof(Vertex);
		const GLvoid *pCoords = &_cone[0].position.x;
		const GLvoid *pColors = &_cone[0].color.x;
	
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, stride, pCoords);
		glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColors);
	
		glDrawArrays(GL_TRIANGLE_STRIP, 0, _cone.size());
	}

	// Draw disk
	{
		GLsizei stride = sizeof(Vertex);
		const GLvoid *pCoords = &_disk[0].position.x;
		const GLvoid *pColors = &_disk[0].color.x;
		
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, stride, pCoords);
		glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColors);
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, _disk.size());
	}
	
	glDisableVertexAttribArray(positionSlot);
	glDisableVertexAttribArray(colorSlot);
}

void RenderingEngine2::updateAnimation(float timeStep)
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

void RenderingEngine2::onRotate(DeviceOrientation orientation)
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

GLuint RenderingEngine2::buildShader(const char *source, GLenum shaderType) const
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	
	GLint compileSuccess;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
	
	if (compileSuccess == GL_FALSE) {
		GLchar message[255];
		glGetShaderInfoLog(shader, sizeof(message), 0, message);
		std::cout << message;
		exit(1);
	}
	
	return shader;
}

GLuint RenderingEngine2::buildProgram(const char *vertexShaderSource, const char *fragmentShaderSource) const
{
	GLuint vertexShader = buildShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragmentShader = buildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	
	GLint linkSuccess; 
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
	
	if (linkSuccess == GL_FALSE) {
		GLchar message[256];
		glGetProgramInfoLog(program, sizeof(message), 0, message);
		std::cout << message;
		exit(1);
	}
	
	return program;
}
