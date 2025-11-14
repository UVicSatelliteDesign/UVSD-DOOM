// This file represents the ground station and will communicate with the
// uvsd-doom by sending it key inputs and displaying the output that is streams
#include <SDL.h>
// More imports

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

// receive input and send commands
