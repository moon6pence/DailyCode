#import "GLView.h"
#import <OpenGLES/ES2/gl.h> // for GL_RENDERBUFFER only

@implementation GLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id) initWithFrame: (CGRect) frame
{
    if (self = [super initWithFrame:frame])
    {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) self.layer;
        eaglLayer.opaque = YES;

        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
        _context = [[EAGLContext alloc] initWithAPI:api];
        
        if (!_context || ![EAGLContext setCurrentContext:_context]) {
            [self release];
            return nil;
        }
        
        _renderingEngine = IRenderingEngine::createRenderingEngine();
        _applicationEngine = IApplicationEngine::createApplicationEngine(_renderingEngine);

        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
                
        int width = CGRectGetWidth(frame);
        int height = CGRectGetHeight(frame);
        _applicationEngine->initialize(width, height);
        
        [self drawView:nil];
        _timestamp = CACurrentMediaTime();
        
        CADisplayLink* displayLink;
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    }
	
    return self;
}

- (void)drawView:(CADisplayLink*)displayLink
{
    if (displayLink != nil) {
        float elapsedSeconds = displayLink.timestamp - _timestamp;
        _timestamp = displayLink.timestamp;
        _applicationEngine->updateAnimation(elapsedSeconds);
    }
    
    _applicationEngine->render();
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* touch = [touches anyObject];
    CGPoint location  = [touch locationInView: self];
    
	_applicationEngine->onMouseDown(ivec2(location.x, location.y));
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* touch = [touches anyObject];
    CGPoint location  = [touch locationInView: self];
	
    _applicationEngine->onMouseUp(ivec2(location.x, location.y));
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* touch = [touches anyObject];
    CGPoint previous  = [touch previousLocationInView: self];
    CGPoint current = [touch locationInView: self];
	
    _applicationEngine->onMouseMove(ivec2(previous.x, previous.y), ivec2(current.x, current.y));
}

@end
