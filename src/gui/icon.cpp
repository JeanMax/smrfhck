#include "gui/gui.hpp"
#include "gui/icon.hpp"


void set_icon(SDL_Window* window)
{
    unsigned char pixel_data[ICON_PIXEL_DATA_SIZE] = ICON_PIXEL_DATA;

    // these masks are needed to tell SDL_CreateRGBSurface(From)
    // to assume the data it gets is byte-wise RGB(A) data
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int shift = (my_icon.bytes_per_pixel == 3) ? 8 : 0;
    rmask = 0xff000000 >> shift;
    gmask = 0x00ff0000 >> shift;
    bmask = 0x0000ff00 >> shift;
    amask = 0x000000ff >> shift;
#else // little endian, like x86
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = (ICON_BYTES_PER_PIXEL == 3) ? 0 : 0xff000000;
#endif

    SDL_Surface* icon = SDL_CreateRGBSurfaceFrom((void *)pixel_data,
                                                 ICON_WIDTH, ICON_HEIGHT,
                                                 ICON_BYTES_PER_PIXEL * 8,
                                                 ICON_BYTES_PER_PIXEL * ICON_WIDTH,
                                                 rmask, gmask, bmask, amask);
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);
}
