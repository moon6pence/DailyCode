//
//  IRenderingEngine.h
//  OpenGL_ES
//
//  Created by khkim on 12. 7. 23..
//  Copyright (c) 2012년 chickenjuice.net. All rights reserved.
//

#ifndef OpenGL_ES_IRenderingEngine_h
#define OpenGL_ES_IRenderingEngine_h

enum DeviceOrientation
{
	DeviceOrientationUnknown, 
	DeviceOrientationPortrait, 
	DeviceOrientationPortraitUpsideDown, 
	DeviceOrientationLandscapeLeft, 
	DeviceOrientationLandscapeRight, 
	DeviceOrientationFaceUp, 
	DeviceOrientationFaceDown, 
};

struct IRenderingEngine *createRenderer1();
struct IRenderingEngine *createRenderer2();

struct IRenderingEngine
{
	virtual ~IRenderingEngine() { };
	
	virtual void initialize(int width, int height) = 0;
	virtual void render() const = 0;
	virtual void updateAnimation(float timeStep) = 0;
	virtual void onRotate(DeviceOrientation orientation) = 0;
};

#endif
