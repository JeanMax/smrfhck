#ifndef _SMRFHCK_HPP
#define _SMRFHCK_HPP

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include <unistd.h> // sleep
#include <thread>
#include <mutex>
#include <chrono>

#include <smrf.h>


#define COLOR_BLACK_1   0.141f,  0.141f, 0.141f  //fg
#define COLOR_BLACK_2   0.376f,  0.376f, 0.376f
#define COLOR_RED_1     0.678f,  0.208f, 0.133f
#define COLOR_RED_2     0.804f,  0.333f, 0.259f
#define COLOR_GREEN_1   0.0667f, 0.514f, 0.282f
// #define COLOR_GREEN_2   0.208f,  0.596f, 0.349f
#define COLOR_GREEN_2   0.317f,  0.686f, 0.435f
#define COLOR_YELLOW_1  0.729f,  0.729f, 0.212f
#define COLOR_YELLOW_2  0.722f,  0.718f, 0.294f
#define COLOR_BLUE_1    0.259f,  0.467f, 0.694f
#define COLOR_BLUE_2    0.322f,  0.561f, 0.82f
#define COLOR_MAGENTA_1 0.678f,  0.353f, 0.824f
#define COLOR_MAGENTA_2 0.749f,  0.478f, 0.867f
#define COLOR_CYAN_1    0.0f,    0.529f, 0.51f
#define COLOR_CYAN_2    0.157f,  0.69f,  0.612f
#define COLOR_WHITE_1   0.741f,  0.741f, 0.702f
#define COLOR_WHITE_2   0.8f,    0.8f,   0.8f    //bg



// graphics.cpp
#define WINDOW_NAME   "smrfhck"
#define WINDOW_WIDTH  480
#define WINDOW_HEIGHT 600

typedef void t_frame_callback(void *data);

BOOL render_loop(t_frame_callback frame_callback, void *data);
void draw_rect(float x, float y, float w, float h, ImColor *color);


#endif
