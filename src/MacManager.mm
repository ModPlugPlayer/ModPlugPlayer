#include <Cocoa/Cocoa.h>
#include "MacManager.h"

void MacManager::removeTitlebarFromWindow(long winId) {
	if(winId == -1)
	{
		QWindowList windows = QGuiApplication::allWindows();
		QWindow* win = windows.first();
		winId = win->winId();
	}

	NSView *nativeView = reinterpret_cast<NSView *>(winId);
	NSWindow* nativeWindow = [nativeView window];
	[nativeWindow setStyleMask:[nativeWindow styleMask] | NSWindowStyleMaskFullSizeContentView];
	[nativeWindow setTitlebarAppearsTransparent:YES];
	[nativeWindow setMovableByWindowBackground:NO];
	[nativeWindow setTitleVisibility:NSWindowTitleVisibility::NSWindowTitleHidden];
}

void MacManager::hideFullscreenButton(long winId){
	if(winId == -1)
	{
		QWindowList windows = QGuiApplication::allWindows();
		QWindow* win = windows.first();
		winId = win->winId();
	}

	NSView *nativeView = reinterpret_cast<NSView *>(winId);
	NSWindow* nativeWindow = [nativeView window];
	/*
	NSButton *button = [nativeWindow standardWindowButton:NSWindowFullScreenButton];
	[button setHidden:YES];
	button.alphaValue = 0.0;
	[button setEnabled:NO];
	button.image = nil;
	button.alternateImage = nil;
	*/
	[nativeWindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenNone];
}
