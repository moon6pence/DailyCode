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

class RenderingEngine1 : public IRenderingEngine
{
public:
	RenderingEngine1();
	
	void initialize(int width, int height);
	void render() const;
	void updateAnimation(float timeStep);
	
private:
	GLuint _frameBuffer;
	GLuint _renderBuffer;
};

IRenderingEngine *createRenderer1()
{
	return new RenderingEngine1();
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

RenderingEngine1::RenderingEngine1()
{
	glGenRenderbuffersOES(1, &_renderBuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, _renderBuffer);
}

void RenderingEngine1::initialize(int width, int height)
{
	glGenFramebuffersOES(1, &_frameBuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, _frameBuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, _renderBuffer);
	
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	
	const float maxX = 2, maxY = 3;
	glOrthof(-maxX, +maxX, -maxY, +maxY, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
}

void RenderingEngine1::render() const
{
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &Vertices[0].position[0]);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &Vertices[0].color[0]);
	
	GLsizei vertexCount = sizeof(Vertices) / sizeof(Vertex);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void RenderingEngine1::updateAnimation(float timestamp)
{
}