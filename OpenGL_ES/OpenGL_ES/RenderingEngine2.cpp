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
#include <cmath>
#include <iostream>

#define STRINGFY(A) #A
#include "Simple.vert"
#include "Simple.frag"

static const float RevolutionsPerSecond = 1;

class RenderingEngine2 : public IRenderingEngine
{
public:
	RenderingEngine2();
	
	void initialize(int width, int height);
	void render() const;
	void updateAnimation(float timeStep);
	void onRotate(DeviceOrientation orientation);
	
private:
	float rotationDirection() const;
	
	GLuint _frameBuffer;
	GLuint _renderBuffer;
	
	float _currentAngle;
	float _desiredAngle;
	
	GLuint buildShader(const char *source, GLenum shaderType) const;
	GLuint buildProgram(const char *vertexShader, const char *fragmentShader) const;
	void applyOrthogonal(float max_x, float max_y) const;
	void applyRotation(float degrees) const;
	
	GLuint _program;
};

IRenderingEngine *createRenderer2()
{
	return new RenderingEngine2();
}

struct Vertex
{
	float position[2];
	float color[4];
};

const Vertex Vertices[] = 
{
	{{-0.5, -0.866}, {1, 1, 0.5f, 1}}, 
	{{0.5, -0.866}, {1, 1, 0.5f, 1}}, 
	{{0, 1}, {1, 1, 0.5f, 1}}, 
	{{-0.5, -0.866}, {0.5f, 0.5f, 0.5f}}, 
	{{0.5, -0.866}, {0.5f, 0.5f, 0.5f}}, 
	{{0, -0.4}, {0.5f, 0.5f, 0.5f}}, 
};

RenderingEngine2::RenderingEngine2()
{
	glGenRenderbuffers(1, &_renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
}

void RenderingEngine2::initialize(int width, int height)
{
	glGenFramebuffers(1, &_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
	
	glViewport(0, 0, width, height);
	
	_program = buildProgram(SimpleVertexShader, SimpleFragmentShader);
	glUseProgram(_program);
	
	applyOrthogonal(2, 3);
		
	onRotate(DeviceOrientationPortrait);
	_currentAngle = _desiredAngle;
}

void RenderingEngine2::render() const
{
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	applyRotation(_currentAngle);
	
	GLuint positionSlot = glGetAttribLocation(_program, "Position");
	GLuint colorSlot = glGetAttribLocation(_program, "SourceColor");
	
	glEnableVertexAttribArray(positionSlot);
	glEnableVertexAttribArray(colorSlot);
	
	GLsizei stride = sizeof(Vertex);
	const GLvoid *pCoords = &Vertices[0].position[0];
	const GLvoid *pColors = &Vertices[0].color[0];
	
	glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, stride, pCoords);
	glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColors);
	
	GLsizei vertexCount = sizeof(Vertices) / sizeof(Vertex);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(positionSlot);
	glDisableVertexAttribArray(colorSlot);
}

void RenderingEngine2::updateAnimation(float timeStep)
{
	float direction = rotationDirection();
	if (direction == 0) return;
	
	float degrees = timeStep * 360 * RevolutionsPerSecond;
	_currentAngle += degrees * direction;
	
	if (_currentAngle >= 360)
		_currentAngle -= 360;
	else if (_currentAngle < 0)
		_currentAngle += 360;
	
	if (rotationDirection() != direction)
		_currentAngle = _desiredAngle;
}

void RenderingEngine2::onRotate(DeviceOrientation orientation)
{
	float angle = 0;
	
	switch (orientation) {
		case DeviceOrientationPortrait:
		case DeviceOrientationFaceUp:
		case DeviceOrientationFaceDown:
			angle = 0;
			break;
			
		case DeviceOrientationLandscapeLeft:
			angle = 270;
			break;
			
		case DeviceOrientationPortraitUpsideDown:
			angle = 180;
			break;
			
		case DeviceOrientationLandscapeRight:
			angle = 90;
			break;
			
		default:
			angle = 0;
			break;
	}
	
	_desiredAngle = angle;
}

float RenderingEngine2::rotationDirection() const
{
	float delta = _desiredAngle - _currentAngle;
	if (delta == 0) return 0;
	
	bool counterclockwise = ((delta > 0 && delta <= 180) || (delta < -180));
	return counterclockwise ? +1 : -1;
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

void RenderingEngine2::applyOrthogonal(float max_x, float max_y) const
{
	float a = 1.0f / max_x;
	float b = 1.0f / max_y;
	float orthogonal[16] = {
		a, 0, 0, 0, 
		0, b, 0, 0, 
		0, 0, -1, 0, 
		0, 0, 0, 1
	};
	
	GLint projectionUniform = glGetUniformLocation(_program, "Projection");
	glUniformMatrix4fv(projectionUniform, 1, 0, &orthogonal[0]);
}

void RenderingEngine2::applyRotation(float degrees) const
{
	float radians = degrees * 3.14159f / 180.0f;
	float s = std::sin(radians);
	float c = std::cos(radians);
	float z_rotation[16] = {
		c, s, 0, 0, 
		-s, c, 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1
	};
	
	GLint modelviewUniform = glGetUniformLocation(_program, "ModelView");
	glUniformMatrix4fv(modelviewUniform, 1, 0, z_rotation);
}
