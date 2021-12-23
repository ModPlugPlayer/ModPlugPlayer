/*
HideTitleBar class declarations and definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

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
