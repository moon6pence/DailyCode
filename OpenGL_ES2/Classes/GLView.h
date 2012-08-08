#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import "Interfaces.hpp"

@interface GLView : UIView {
    IApplicationEngine *_applicationEngine;
    IRenderingEngine *_renderingEngine;
    EAGLContext *_context;
    float _timestamp;
}

- (void)drawView:(CADisplayLink*)displayLink;

@end
