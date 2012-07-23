//
//  AppDelegate.m
//  OpenGL_ES
//
//  Created by khkim on 12. 7. 23..
//  Copyright (c) 2012년 chickenjuice.net. All rights reserved.
//

#import "AppDelegate.h"
#import "GLView.h"

@implementation AppDelegate
@synthesize window = _window, view = _view;

- (void)dealloc
{
	[_window release];
	[_view release];
	
    [super dealloc];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	
    self.window = [[[UIWindow alloc] initWithFrame:screenBounds] autorelease];
	self.view = [[[GLView alloc] initWithFrame:screenBounds] autorelease];
	
	[self.window addSubview:self.view];
	[self.window makeKeyAndVisible];

    return YES;
}

@end
