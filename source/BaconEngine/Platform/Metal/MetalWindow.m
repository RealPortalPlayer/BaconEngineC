// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <Cocoa/Cocoa.h>

#include "MetalWindow.h"

SEC_CPP_SUPPORT_GUARD_START()
NSWindow* beMetalWindow;

void* BE_MetalWindow_GetWindow(void) {
    return (__bridge void*) beMetalWindow;
}

const char* BE_MetalWindow_GetTitle(void) {
    return [[beMetalWindow title] UTF8String];
}

BE_Vector_2U BE_MetalWindow_GetSize(void) {
    CGSize size = [[beMetalWindow contentView] frame].size;
    return SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2U, (unsigned) size.width, (unsigned) size.height);
}

BE_Vector_2I BE_MetalWindow_GetPosition(void) {
    CGPoint position = [[beMetalWindow contentView] frame].origin;
    return SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2I, (int) position.x, (int) position.y);
}

SEC_Boolean BE_MetalWindow_IsVisible(void) {
    return [beMetalWindow isVisible];
}

SEC_Boolean BE_MetalWindow_IsStillOpened(void) {
    return SEC_TRUE; // TODO
}

void BE_MetalWindow_Create(const char* title, BE_Vector_2U size, int monitor) {

}

void BE_MetalWindow_SetTitle(const char* title) {
    [beMetalWindow setTitle:[NSString stringWithUTF8String:title]];
}

void BE_MetalWindow_SetSize(BE_Vector_2U newSize) {
    NSRect rectangle = [beMetalWindow frame];

    rectangle.origin.y += (rectangle.size.height - newSize.y);
    rectangle.size = CGSizeMake(newSize.x, newSize.y);

    [beMetalWindow setFrame:rectangle display:YES];
}

void BE_MetalWindow_SetPosition(BE_Vector_2I newPosition) {
}

void BE_MetalWindow_SetVisibility(SEC_Boolean visible) {
    [beMetalWindow setIsVisible:(BOOL) visible];
}

void BE_MetalWindow_Close(void) {
    [beMetalWindow close];
}

void BE_MetalWindow_UpdateEvents(void) {
    [beMetalWindow update];
}
SEC_CPP_SUPPORT_GUARD_END()
