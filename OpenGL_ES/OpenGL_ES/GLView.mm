//
//  GLView.m
//  OpenGL_ES
//
//  Created by khkim on 12. 7. 23..
//  Copyright (c) 2012년 chickenjuice.net. All rights reserved.
//

#import "GLView.h"

#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import "IRenderingEngine.h"

@interface GLView () {
	EAGLContext *_context;
	IRenderingEngine *_renderingEngine;
	float _timestamp;
}

- (void)drawView:(CADisplayLink *)displayLink;
- (void)didRotate:(NSNotification *)notification;

@end

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
		
		EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
		_context = [[EAGLContext alloc] initWithAPI:api];
		
		if (nil == _context || false == [EAGLContext setCurrentContext:_context]) {
			[self release];
			return nil;
		}

		_renderingEngine = IRenderingEngine::createRenderer();
		
		[_context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
		
		_renderingEngine->initialize(CGRectGetWidth(frame), CGRectGetHeight(frame));
		
		[self drawView:nil];
		_timestamp = CACurrentMediaTime();
		
		CADisplayLink *displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
		[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		
		[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
		
		[[NSNotificationCenter defaultCenter] addObserver:self 
												 selector:@selector(didRotate:) 
													 name:UIDeviceOrientationDidChangeNotification 
												   object:nil];
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

- (void)drawView:(CADisplayLink *)displayLink
{
	if (displayLink != nil) {
		float elaspedSeconds = displayLink.timestamp - _timestamp;
		_timestamp = displayLink.timestamp;
		_renderingEngine->updateAnimation(elaspedSeconds);
	}
	
	_renderingEngine->render();
	[_context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void)didRotate:(NSNotification *)notification
{
	UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
	_renderingEngine->onRotate((DeviceOrientation)orientation);
	[self drawView:nil];
}

@end
