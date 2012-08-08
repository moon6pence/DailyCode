//
//  SimpleWireframeAppDelegate.m
//  SimpleWireframe
//
//  Created by Brian Jepson on 2/23/10.
//  Copyright O'Reilly Media 2010. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(UIApplication*)application
{
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    
    _window = [[UIWindow alloc] initWithFrame:screenBounds];
    _view = [[GLView alloc] initWithFrame:screenBounds];
    
    [_window addSubview:_view];
    [_window makeKeyAndVisible];
}

- (void)dealloc
{
    [_view release];
    [_window release];
	
    [super dealloc];
}

@end
