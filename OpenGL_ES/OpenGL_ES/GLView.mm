//
//  GLView.m
//  OpenGL_ES
//
//  Created by khkim on 12. 7. 23..
//  Copyright (c) 2012년 chickenjuice.net. All rights reserved.
//

#import "GLView.h"

@implementation GLView

+ (Class)layerClass
{
	return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
		CAEAGLLayer *eaglLayer = (CAEAGLLayer *)super.layer;
		eaglLayer.opaque = YES;
		
		_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		
		if (nil == _context || false == [EAGLContext setCurrentContext:_context]) {
			[self release];
			return nil;
		}
		
		GLuint frameBuffer, renderBuffer;
		glGenFramebuffersOES(1, &frameBuffer);
		glGenRenderbuffersOES(1, &renderBuffer);
		
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, frameBuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderBuffer);
		
		[_context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
		
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, 
									 GL_RENDERBUFFER_OES, renderBuffer);
		
		glViewport(0, 0, CGRectGetWidth(frame), CGRectGetHeight(frame));
		
		[self drawView];
    }
    return self;
}

- (void)dealloc
{
	if ([EAGLContext currentContext] == _context)
		[EAGLContext setCurrentContext:nil];
	
	[_context release];
	
	[super dealloc];
}

- (void)drawView
{
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	[_context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

@end
