#include <Cocoa/Cocoa.h>
#include "MacManager.h"

inline NSWindow * getNSWindow(long winId = -1) {
	if(winId == -1)
	{
		QWindowList windows = QGuiApplication::allWindows();
		QWindow* win = windows.first();
		winId = win->winId();
	}

	NSView *nativeView = reinterpret_cast<NSView *>(winId);
	NSWindow* nativeWindow = [nativeView window];
	return nativeWindow;
}

inline NSButton * getZoomButton(NSWindow * nsWindow){
	return [nsWindow standardWindowButton:NSWindowZoomButton];

}

inline void hideButton(NSButton *button) {
	[button setHidden:YES];
	button.alphaValue = 0.0;
	[button setEnabled:NO];
	button.image = nil;
	button.alternateImage = nil;
}

inline void disableButton(NSButton *button) {
	[button setHidden:NO];
	[button setEnabled:NO];
}

void MacManager::removeTitlebarFromWindow(long winId) {
	NSWindow * nativeWindow = getNSWindow(winId);
	[nativeWindow setStyleMask:[nativeWindow styleMask] | NSWindowStyleMaskFullSizeContentView];
	[nativeWindow setTitlebarAppearsTransparent:YES];
	[nativeWindow setMovableByWindowBackground:NO];
	[nativeWindow setTitleVisibility:NSWindowTitleVisibility::NSWindowTitleHidden];
}

void MacManager::hideZoomButton(long winId){
	NSWindow* nativeWindow = getNSWindow(winId);
	NSButton *button = getZoomButton(nativeWindow);
	hideButton(button);
}

void MacManager::disableZoomButton(long winId){
	NSWindow* nativeWindow = getNSWindow(winId);
	NSButton *button = getZoomButton(nativeWindow);
	disableButton(button);
}

void MacManager::nonFunctionalZoomButton(long winId)
{
	NSWindow* nativeWindow = getNSWindow(winId);
	[nativeWindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenNone];
}

