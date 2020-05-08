#include "hidetitlebar.h"

#import <AppKit/AppKit.h>

void HideTitleBar::hideTitleBar(QWindow *window)
{
    NSView *nativeView = reinterpret_cast<NSView *>(window->winId());
    NSWindow* nativeWindow = nativeView.window;

    // Default masks
    nativeWindow.styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;

    // Content under title bar
    nativeWindow.styleMask |= NSWindowStyleMaskFullSizeContentView;

    nativeWindow.titlebarAppearsTransparent = true;
    //nativeWindow.movableByWindowBackground = true;
    //nativeWindow.titleVisibility = NSWindowTitleHidden; // Hide window title name
}
