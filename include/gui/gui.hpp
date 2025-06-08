#ifndef _GUI_HPP
#define _GUI_HPP

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include <smrf.h>

#include "settings.hpp"



typedef void t_frame_callback(void *data);

// graphics.cpp
bool render_loop(t_frame_callback frame_callback, void *data);

// icon.cpp
void set_icon(SDL_Window *window);

// config.cpp
void read_window_title_config(GameState *game);
int *read_window_config(void);
void read_config(void);
void write_config(SDL_Window *window, GameState *game);

// draw.cpp
void draw_circle(float x, float y, float radius, ImColor *color);
void draw_rect(float x, float y, float w, float h, ImColor *color);
void draw(float x, float y, Setting *s);


#endif
