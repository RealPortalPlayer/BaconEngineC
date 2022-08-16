// Purpose: Make mock SDL structs when SDL is disabled.
// Created on: 8/15/22 @ 12:42 PM

#pragma once

#ifndef BACON_ENGINE_DISABLE_SDL
#   include <SDL.h>
#   include <SDL_events.h>
#else
    typedef struct {
        void* placeholder;
    } SDL_Event;

    typedef struct {
        void* placeholder;
    } SDL_Window;

    typedef struct {
        void* placeholder;
    } SDL_Renderer;

    typedef struct {
        void* placeholder;
    } SDL_Scancode;
#endif

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
#   include <SDL_ttf.h>
#else
    typedef struct {
        void* placeholder;
    } TTF_Font;
#endif
