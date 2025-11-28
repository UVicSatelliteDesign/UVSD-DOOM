#include "doomgeneric.h"
#include "doomkeys.h"
#include "m_argv.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <stdbool.h>

void DG_Init() {
  // Initialize your platfrom (create window, framebuffer, etc...).
}

void DG_DrawFrame(uint32_t ms) {
  // Frame is ready in DG_ScreenBuffer. Copy it to your platform's screen.
}

void DG_SleepMs() {
  // sleep for ms
}

uint32_t DG_GetTicksMs() {
  // The ticks passed since launch in milliseconds.
}

#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static unsigned char convertToDoomKey(unsigned int key) {
  switch (key) {
  case SDLK_RETURN:
    key = KEY_ENTER;
    break;
  case SDLK_ESCAPE:
    key = KEY_ESCAPE;
    break;
  case SDLK_LEFT:
    key = KEY_LEFTARROW;
    break;
  case SDLK_RIGHT:
    key = KEY_RIGHTARROW;
    break;
  case SDLK_UP:
    key = KEY_UPARROW;
    break;
  case SDLK_DOWN:
    key = KEY_DOWNARROW;
    break;
  case SDLK_LCTRL:
  case SDLK_RCTRL:
    key = KEY_FIRE;
    break;
  case SDLK_SPACE:
    key = KEY_USE;
    break;
  case SDLK_LSHIFT:
  case SDLK_RSHIFT:
    key = KEY_RSHIFT;
    break;
  case SDLK_LALT:
  case SDLK_RALT:
    key = KEY_LALT;
    break;
  case SDLK_F2:
    key = KEY_F2;
    break;
  case SDLK_F3:
    key = KEY_F3;
    break;
  case SDLK_F4:
    key = KEY_F4;
    break;
  case SDLK_F5:
    key = KEY_F5;
    break;
  case SDLK_F6:
    key = KEY_F6;
    break;
  case SDLK_F7:
    key = KEY_F7;
    break;
  case SDLK_F8:
    key = KEY_F8;
    break;
  case SDLK_F9:
    key = KEY_F9;
    break;
  case SDLK_F10:
    key = KEY_F10;
    break;
  case SDLK_F11:
    key = KEY_F11;
    break;
  case SDLK_EQUALS:
  case SDLK_PLUS:
    key = KEY_EQUALS;
    break;
  case SDLK_MINUS:
    key = KEY_MINUS;
    break;
  default:
    key = tolower(key);
    break;
  }

  return key;
}

static void addKeyToQueue(int pressed, unsigned int keyCode) {
  unsigned char key = convertToDoomKey(keyCode);

  unsigned short keyData = (pressed << 8) | key;

  s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
  s_KeyQueueWriteIndex++;
  s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}
static void handleKeyInput() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      puts("Quit requested");
      atexit(SDL_Quit);
      exit(1);
    }
    if (e.type == SDL_KEYDOWN) {
      // KeySym sym = XKeycodeToKeysym(s_Display, e.xkey.keycode, 0);
      // printf("KeyPress:%d sym:%d\n", e.xkey.keycode, sym);
      addKeyToQueue(1, e.key.keysym.sym);
    } else if (e.type == SDL_KEYUP) {
      // KeySym sym = XKeycodeToKeysym(s_Display, e.xkey.keycode, 0);
      // printf("KeyRelease:%d sym:%d\n", e.xkey.keycode, sym);
      addKeyToQueue(0, e.key.keysym.sym);
    }
  }
}

// Provide keyboard events.
int DG_GetKey(int *pressed, unsigned char *doomKey) {
  if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex) {
    // key queue is empty
    return 0;
  } else {
    unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
    s_KeyQueueReadIndex++;
    s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

    *pressed = keyData >> 8;
    *doomKey = keyData & 0xFF;

    return 1;
  }

  return 0;
}

// input mechanism for testing key inputs
void test_input_system() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL init failed: %s\n", SDL_GetError());
    return;
  }

  // Creates an SDL window with the following parts
  SDL_Window *window = SDL_CreateWindow(
      "Doom Input Test",      // Window title
      SDL_WINDOWPOS_CENTERED, // X position (centered on screen)
      SDL_WINDOWPOS_CENTERED, // Y position (centered on screen)
      640, 480,               // Width, height in pixels
      SDL_WINDOW_SHOWN        // Show window immediately
  );

  int running = 1;
  while (running) {
    handleKeyInput();

    // Process all queued keys
    while (s_KeyQueueReadIndex != s_KeyQueueWriteIndex) {
      unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
      int pressed = (keyData >> 8) & 0xFF; // Extract press/release flag
      int key = keyData & 0xFF;            // Extract key code

      printf("%s: %d (0x%02X)\n", pressed ? "PRESS  " : "RELEASE", key, key);

      if (key == KEY_ESCAPE)
        running = 0; // Exit on ESC

      s_KeyQueueReadIndex++;
      s_KeyQueueReadIndex %= KEYQUEUE_SIZE; // Wrap around with mod
    }

    SDL_Delay(16); // Sleep ~16ms which is about 60fps
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char *argv[]) {
  //
  if (argc > 1 && strcmp(argv[1], "--test-input") == 0) {
    test_input_system();
    return 0;
  }

  // Normal Doom startup will go here
  printf("Starting Doom...\n");

  doomgeneric_Create(argc, argv);

  for (int i = 0;; i++) {
    doomgeneric_Tick();
  }

  return 0;
}
